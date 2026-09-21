/*
 *  Copyright: Bálint Kurucz - Danube Mechatronics Kft.
 *
 *  Available under dual-license A) GPL | B) Commercial.
 *  Accept one, ignore the other.
 *  For GPL see License.md.
 *  For commercial, contact me:
 *  kb@danube-mechatronics.com
 *
 *  Keep this here if you choose A).
 */

#ifndef A2LFILE_H
#define A2LFILE_H

#include "a2l/detail/number.h"

#include <cctype>
#include <cstdint>
#include <fstream>
#include <istream>
#include <map>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace a2lfile
{

inline const std::string asap2ver = "ASAP2_VERSION";
inline const std::string blockBegin = "/begin";
inline const std::string blockEnd = "/end";
/* Blocks whose body is preserved as text instead of tokenized. */
inline const std::unordered_set<std::string> preservedBlockNames = {"A2ML"};
inline const std::string commentBegin = "/*";
inline const std::string commentEnd = "*/";
inline const std::string lineComment = "//";

struct Block;

struct LineItem
{
    enum Type
    {
        Invalid, /* Added for convinience */
        String, /* MCD */
        Decimal,  /* MCD */
        Hex,  /* MCD */
        Float,  /* MCD */
        Identifier /* MCD -  not quoted text - ie Keywords */
    };

    /* An Invalid item links to itself, so walking past a missing token
       terminates. Only the shared sentinel is ever Invalid: stored tokens live
       by value inside their block and are linked when the block closes. */
    LineItem(Type type, std::string txt)
        : _next(type == Invalid ? this : nullptr), _type(type), _txt(std::move(txt))
    {}

    /* The one missing-token item. Immutable, constructed once, self-linked. */
    static const LineItem* invalid()
    {
        static const LineItem sentinel(Invalid, "Invalid!");
        return &sentinel;
    }

    /* Links a finished run of tokens into one forward chain that ends at the
       shared Invalid item. Called once the run stops growing, because the
       addresses it stores must stay put. */
    static void link(std::vector<LineItem>& items)
    {
        for (size_t i = 0; i + 1 < items.size(); i++)
            items[i]._next = &items[i + 1];
        if (!items.empty())
            items.back()._next = invalid();
    }

    /* The token's text: a quoted string without its delimiters, any other
       token exactly as it was written. */
    const std::string& toText() const { return _txt; }

    uint64_t toUnsigned() const { return detail::readNumber<uint64_t>(*this).value; }
    int64_t toSigned() const { return detail::readNumber<int64_t>(*this).value; }
    double toDouble() const { return detail::readNumber<double>(*this).value; }

    struct Iterator
    {
        Iterator( const LineItem* li ) : _li(li)
        {};

        bool isEnd() const
        {
            return ( _li->_type == Invalid );
        };

        /* prefix ++ */
        Iterator& operator++ ()
        {
            _li = _li->_next;
            return *this;
        }

        /* postfix ++ */
        Iterator operator++ (int)
        {
            Iterator ret = *this;
            ++(*this);
            return ret;
        }

        const LineItem* li() const { return _li; }
      private:
        const LineItem* _li;
    };

    const LineItem* next() const { return _next; };
    Type type() const { return _type; };

  private:
    const LineItem* _next;
    Type _type;
    std::string _txt;
};

/* The single lexical authority for A2L source text. It decides where comments,
   quoted strings, /begin, /end and value tokens start and end, and which value
   class a token belongs to; nothing above it re-reads source characters.

   Both consumers -- the token stream and the verbatim copy of a preserved block
   body -- advance through the same take() units, so they cannot disagree about
   where a string or a comment ends. Only what each does with a unit differs.

   One physical line is held at a time -- a quoted string or a block comment
   continues by refilling that buffer -- so the source is never retained whole.
   Every take() consumes at least one character, refills a line, or reports End,
   so a bounded input always terminates. */
class Scanner
{
  public:
    /* What the scanner just passed. next() reports only the four that carry
       structure; the other three exist because the preserved-body copy walks
       the same units. */
    enum Lexeme { End, LineBreak, Space, Comment, BeginBlock, EndBlock, Value };

    explicit Scanner(std::istream& source) : _source(source) {}

    /* The next lexeme that carries structure: End, BeginBlock, EndBlock or Value.

       Everything between a /begin and the name that follows it is also written
       to the block header, because a preserved body starts there and the scanner
       never reads a character twice: input is only ever consumed forward, so a
       comment that closed between the two cannot come back as active syntax. */
    Lexeme next()
    {
        for (;;)
        {
            const Lexeme lexeme = take(_in_header ? &_header : nullptr);
            if (lexeme == LineBreak || lexeme == Space || lexeme == Comment)
                continue;
            if (lexeme == BeginBlock)
            {
                _header.clear();
                _in_header = true;
            }
            else
            {
                _in_header = false;
            }
            return lexeme;
        }
    }

    const std::string& text() const { return _text; }
    LineItem::Type type() const { return _type; }

    /* Body of a block that is preserved instead of tokenized (A2ML). It opens
       with the header already scanned for this block -- the separators and the
       name that followed its /begin -- and continues forward from the cursor:
       every following line, comments collapsed to the separator they stood for
       and blank lines dropped, up to the /end that closes the block. The body is
       opaque: a nested /begin inside it is text, not a child block. The closing
       /end is left for the next next() to report, and text before it on the same
       line is kept. */
    std::string takeBlockBody()
    {
        std::string body;
        std::string line = _header;
        auto flush = [&body, &line] {
            if (line.find_first_not_of(" \t\n\v\f\r") != std::string::npos)
            {
                body += line;
                body += '\n';
            }
            line.clear();
        };

        for (;;)
        {
            const size_t mark = _cursor;
            const Lexeme lexeme = take(&line);
            if (lexeme == BeginBlock) { line += blockBegin; continue; }
            if (lexeme == LineBreak) { flush(); continue; }
            if (lexeme == EndBlock) { _cursor = mark; break; }
            if (lexeme == End) break;
        }
        flush();
        return body;
    }

  private:
    /* Advances past exactly one lexeme. `sink`, when given, receives the source
       text the lexeme stood for: a block comment collapses to the one space that
       separated its neighbours, a line comment and a line break contribute
       nothing, a quoted run keeps its delimiters, everything else is verbatim.
       /begin and /end are never written to the sink -- whether a delimiter
       belongs in the output is the caller's decision, not a lexical one.
       A Value also fills _text and _type. */
    Lexeme take(std::string* sink)
    {
        if (_cursor >= _line.size())
            return refill() ? LineBreak : End;

        if (_in_comment)
        {
            const size_t close = _line.find(commentEnd, _cursor);
            if (close == std::string::npos)
            {
                _cursor = _line.size(); /* the comment body continues on the next line */
            }
            else
            {
                _in_comment = false;
                _cursor = close + commentEnd.size();
                if (sink) *sink += ' ';
            }
            return Comment;
        }

        if (isSpace(_line[_cursor]))
        {
            const size_t start = _cursor;
            while (_cursor < _line.size() && isSpace(_line[_cursor])) _cursor++;
            if (sink) sink->append(_line, start, _cursor - start);
            return Space;
        }
        if (_line.compare(_cursor, lineComment.size(), lineComment) == 0)
        {
            _cursor = _line.size(); /* nothing of a line comment survives */
            return Comment;
        }
        if (_line.compare(_cursor, commentBegin.size(), commentBegin) == 0)
        {
            _in_comment = true;
            _cursor += commentBegin.size();
            return Comment;
        }
        if (isDelimiterAt(_cursor, blockBegin))
        {
            _cursor += blockBegin.size();
            return BeginBlock;
        }
        if (isDelimiterAt(_cursor, blockEnd))
        {
            _cursor += blockEnd.size();
            return EndBlock;
        }
        scanValue(sink);
        return Value;
    }

    static bool isSpace(char c)
    {
        return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
    }

    bool startsComment(size_t i) const
    {
        return _line.compare(i, commentBegin.size(), commentBegin) == 0 ||
               _line.compare(i, lineComment.size(), lineComment) == 0;
    }

    /* A bare token runs until whitespace, a quote or a comment. A comment
       therefore separates tokens that are otherwise adjacent. */
    bool breaksToken(size_t i) const
    {
        return isSpace(_line[i]) || _line[i] == '"' || startsComment(i);
    }

    /* /begin and /end are whole tokens, never a prefix of a longer one. */
    bool isDelimiterAt(size_t i, const std::string& word) const
    {
        if (_line.compare(i, word.size(), word) != 0) return false;
        const size_t after = i + word.size();
        return after >= _line.size() || breaksToken(after);
    }

    bool refill()
    {
        if (!std::getline(_source, _line))
        {
            _line.clear();
            _cursor = 0;
            return false;
        }
        /* getline drops the LF; a CRLF stream leaves the CR behind. A physical
           line ending is not content -- a continued string joins without it --
           so it is removed here, at the one place lines enter the scanner.
           A literal \r escape inside a string is spelled, not encoded, and is
           untouched. */
        if (!_line.empty() && _line.back() == '\r') _line.pop_back();
        /* A UTF-8 byte order mark is an encoding artifact, not part of a token. */
        if (_line_number == 0 && _line.compare(0, 3, "\xEF\xBB\xBF") == 0)
            _line.erase(0, 3);
        _cursor = 0;
        _line_number++;
        return true;
    }

    /* Fills _text with the token's text -- a quoted string without its
       delimiters, anything else exactly as written -- and appends the token's
       source spelling to `sink`. */
    void scanValue(std::string* sink)
    {
        _text.clear();
        if (_line[_cursor] == '"')
        {
            scanQuoted(sink);
            _type = LineItem::String;
            return;
        }
        const size_t start = _cursor;
        while (_cursor < _line.size() && !breaksToken(_cursor)) _cursor++;
        _text.append(_line, start, _cursor - start);
        if (sink) sink->append(_line, start, _cursor - start);
        _type = classify(_text);
    }

    /* A string runs from its opening quote to the first closing quote that is
       neither escaped -- "\x" is one unit -- nor doubled -- "" is an embedded
       quote, the ASAP2 V1.2 form. The token keeps the source spelling between
       the delimiters, so escapes survive into raw output unchanged. An
       unterminated string continues on the next line, joined without the break,
       and ends at end of input. */
    void scanQuoted(std::string* sink)
    {
        if (sink) *sink += '"';
        _cursor++;
        for (;;)
        {
            if (_cursor >= _line.size())
            {
                if (!refill()) return;
                continue;
            }
            const char c = _line[_cursor];
            if (c == '\\')
            {
                const size_t width = (_cursor + 1 < _line.size()) ? 2 : 1;
                _text.append(_line, _cursor, width);
                if (sink) sink->append(_line, _cursor, width);
                _cursor += width;
                continue;
            }
            if (c == '"')
            {
                if (_cursor + 1 < _line.size() && _line[_cursor + 1] == '"')
                {
                    _text.append(_line, _cursor, 2);
                    if (sink) sink->append(_line, _cursor, 2);
                    _cursor += 2;
                    continue;
                }
                _cursor++;
                if (sink) *sink += '"';
                return;
            }
            const size_t start = _cursor;
            while (_cursor < _line.size() && _line[_cursor] != '"' && _line[_cursor] != '\\')
                _cursor++;
            _text.append(_line, start, _cursor - start);
            if (sink) sink->append(_line, start, _cursor - start);
        }
    }

    /* A hex spelling is a 0x prefix; whether its digits convert is the numeric
       reader's decision, not the scanner's. */
    static bool isHexSpelling(const std::string& t)
    {
        const size_t i = (!t.empty() && (t[0] == '+' || t[0] == '-')) ? 1 : 0;
        return t.compare(i, 2, "0x") == 0 || t.compare(i, 2, "0X") == 0;
    }

    /* Decimal floats and scientific notation (1.5, -1e24, 2.5e-003, 1E-6).
       A float must contain a '.' or an exponent; pure integers stay Decimal.
       Grammar: [sign] digits [ '.' digits ] [ ('e'|'E') [sign] digits ].
       Either a fractional part or an exponent is required; a leading digit run
       is required (".5" and "E5" are rejected as malformed). */
    static bool isFloatSpelling(const std::string& t)
    {
        size_t i = 0;
        const size_t n = t.size();
        if (n == 0) return false;

        if (t[i] == '+' || t[i] == '-') i++;

        size_t intDigits = 0;
        while (i < n && std::isdigit(static_cast<unsigned char>(t[i]))) { i++; intDigits++; }
        if (intDigits == 0) return false;

        bool hasFraction = false;
        if (i < n && t[i] == '.')
        {
            i++;
            hasFraction = true;
            while (i < n && std::isdigit(static_cast<unsigned char>(t[i]))) i++;
        }

        bool hasExponent = false;
        if (i < n && (t[i] == 'e' || t[i] == 'E'))
        {
            i++;
            hasExponent = true;
            if (i < n && (t[i] == '+' || t[i] == '-')) i++;
            size_t expDigits = 0;
            while (i < n && std::isdigit(static_cast<unsigned char>(t[i]))) { i++; expDigits++; }
            if (expDigits == 0) return false; /* malformed: "1E", "1e-" */
        }

        if (i != n) return false;            /* trailing junk */
        return hasFraction || hasExponent;   /* a bare integer is Decimal, not Float */
    }

    static bool isDecimalSpelling(const std::string& t)
    {
        /* dot . is not included: a dotted spelling is a Float or an Identifier */
        return t.find_first_not_of("0123456789+-") == std::string::npos;
    }

    static LineItem::Type classify(const std::string& t)
    {
        if (isHexSpelling(t)) return LineItem::Hex;
        if (isFloatSpelling(t)) return LineItem::Float;
        if (isDecimalSpelling(t)) return LineItem::Decimal;
        return LineItem::Identifier;
    }

    std::istream& _source;
    std::string _line;
    std::string _text;
    std::string _header;      /* what followed the most recent /begin, up to its name */
    size_t _cursor = 0;
    uint64_t _line_number = 0;
    bool _in_header = false;
    bool _in_comment = false;
    LineItem::Type _type = LineItem::Invalid;
};

struct Block
{
    Block(Block* parent) { _parent = parent; }

    void addChild(std::unique_ptr<Block> b) { _children.push_back(std::move(b)); }

    const Block* parent() const { return _parent; }

    const LineItem* liByIdx(uint64_t idx) const
    {
        if (idx < _line_items.size())
            return &_line_items[idx];
        return LineItem::invalid();
    }

    std::vector<const LineItem*> lisByTxtAndType(std::string name, LineItem::Type type) const
    {
        std::vector<const LineItem*> ret;
        for (auto& li : this->_line_items)
        {
            if ((li.toText() == name) && (li.type() == type))
            {
                ret.push_back(&li);
            }
        }
        return ret;
    }

    const LineItem* liByIdent(std::string name) const
    {
        for (auto& li : this->_line_items)
        {
            if (li.type() == LineItem::Identifier)
            {
                if (li.toText() == name) return &li;
            }
        }
        return LineItem::invalid();
    }

    /* Returns the next item (right) to the line item with the name */
    const LineItem* liByIdentNxt(std::string name) const
    {
        return liByIdent(name)->next();
    }

    const LineItem* firstLineItem() const
    {
        return liByIdx(0);
    }

    const Block* childBlockByName(std::string name) const
    {
        const auto itr = _children_lookup.find(name);
        return (itr != _children_lookup.end()) ? itr->second : nullptr;
    }

    std::vector<const Block*> childBlocksByName(std::string name) const
    {
        std::vector<const Block*> ret;
        for (auto [itr, rangeEnd] = _children_lookup.equal_range(name); itr != rangeEnd; ++itr)
        {
            ret.push_back(itr->second);
        }
        return ret;
    }

    /* Appends one scanned token. Only the Loader calls this, and only while the
       block is open: item addresses are handed out after close(). */
    void addToken(LineItem::Type type, std::string txt)
    {
        _line_items.emplace_back(type, std::move(txt));
    }

    /* Ends the block: links its tokens into one forward chain terminated by the
       shared Invalid item, and registers the block under its name in its parent. */
    void close()
    {
        LineItem::link(_line_items);
        if (_parent != nullptr)
            _parent->_children_lookup.insert(std::pair<std::string, Block*>(_name, this));
    }

    std::string _name;
    std::string _raw_content; // Body text of a preserved block (e.g. A2ML).

    Block* _parent;
    std::vector<std::unique_ptr<Block>> _children;

    /* The block's own tokens, contiguous and owned. */
    std::vector<LineItem> _line_items;
    std::multimap<std::string, Block*> _children_lookup;
};

struct A2lFile
{
    /* The ASAP2_VERSION keyword and its two operands, empty when the file
       carries no version line. */
    std::vector<LineItem> ASAP2_VERSION;
    std::unique_ptr<Block> PROJECT;

    std::pair<uint32_t, uint32_t> version() const
    {
        std::pair<uint32_t, uint32_t> ret;
        if( ASAP2_VERSION.size() != 3 )
            return ret;

        ret.first = detail::readNumber<uint32_t>(ASAP2_VERSION[1]).value;
        ret.second = detail::readNumber<uint32_t>(ASAP2_VERSION[2]).value;
        return ret;
    };
};

struct Loader
{
    /* Reads an A2L file. Returns nullptr only when the file cannot be opened;
       any content that scans yields a tree, truncated input included. */
    static std::unique_ptr<A2lFile> readA2lFile(std::string path)
    {
        std::ifstream infile(path);

        if (!infile.is_open())
            return nullptr;

        return read(infile);
    }

    /* Builds the block tree from a scanned token stream. The scanner owns every
       lexical decision; this loop only tracks nesting and where a token belongs. */
    static std::unique_ptr<A2lFile> read(std::istream& source)
    {
        Scanner scanner(source);
        auto af = std::make_unique<A2lFile>();
        std::unique_ptr<Block> root;
        Block* current = nullptr;

        /* What the next value token means. A /begin is followed by the block
           name, a /end by the name it closes; everything else is content. */
        enum { Content, BlockName, EndName } expect = Content;

        for (;;)
        {
            const Scanner::Lexeme lexeme = scanner.next();
            if (lexeme == Scanner::End)
                break;

            if (lexeme == Scanner::BeginBlock)
            {
                auto block = std::make_unique<Block>(current);
                Block* opened = block.get();
                if (current == nullptr)
                    root = std::move(block);
                else
                    current->addChild(std::move(block));
                current = opened;
                expect = BlockName;
                continue;
            }

            if (lexeme == Scanner::EndBlock)
            {
                expect = EndName;
                if (current == nullptr)
                    continue;
                current->close();
                current = current->_parent;
                if (current == nullptr)
                    break; /* the outermost block closed; the rest is not A2L */
                continue;
            }

            if (expect == BlockName)
            {
                expect = Content;
                current->_name = scanner.text();
                if (preservedBlockNames.find(current->_name) != preservedBlockNames.end())
                    current->_raw_content = scanner.takeBlockBody();
                continue;
            }
            if (expect == EndName)
            {
                expect = Content;
                continue; /* the closing name is not content */
            }

            if (current == nullptr)
            {
                /* Before the outermost block only ASAP2_VERSION is recognised. */
                if (scanner.text() == asap2ver)
                {
                    af->ASAP2_VERSION.clear();
                    af->ASAP2_VERSION.emplace_back(scanner.type(), scanner.text());
                }
                else if (!af->ASAP2_VERSION.empty() && af->ASAP2_VERSION.size() < 3)
                {
                    af->ASAP2_VERSION.emplace_back(scanner.type(), scanner.text());
                }
                continue;
            }

            current->addToken(scanner.type(), scanner.text());
        }

        /* Truncated input: close whatever is still open so the tree holds what
           the file did define. */
        while (current != nullptr)
        {
            current->close();
            current = current->_parent;
        }

        LineItem::link(af->ASAP2_VERSION);
        af->PROJECT = std::move(root);
        return af;
    }
};
} // namespace a2lfile

#endif // A2LFILE_H
