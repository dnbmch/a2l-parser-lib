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

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace a2lfile
{

/* ToDo move them to a class */
inline const std::string asap2ver = "ASAP2_VERSION";
inline const std::string blockBegin = "/begin";
inline const std::string blockEnd = "/end";
inline const std::unordered_set<std::string> ignoredBlockNames = {"A2ML"};
inline const std::string commentBegin = "/*";
inline const std::string commentEnd = "*/";
inline const std::string lineComment = "//";

inline const std::vector<char> decimalChars = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '-'}; /* dot . is not included */
inline const std::string decimalF = std::string(decimalChars.begin(), decimalChars.end());

inline const std::vector<char> wpChars = {0x09 /*HT*/, 0x0A /*LF/NL*/, 0x0B /*VT*/, 0x0C /*FF/NP*/, 0x0D /*CR*/, 0x20 /*SPC*/};
inline const std::string wpF = std::string(wpChars.begin(), wpChars.end());

inline bool isString(std::string& str)
{
    return str.find("\"") != std::string::npos;
}

inline bool isHex(std::string& str)
{
    return str.find("0x") != std::string::npos;
}

/* Accept decimal floats and scientific notation (1.5, -1e24, 2.5e-003, 1E-6).
   A float must contain a '.' or an exponent; pure integers stay Decimal.
   Grammar: [sign] digits [ '.' digits ] [ ('e'|'E') [sign] digits ].
   Either a fractional part or an exponent is required; a leading digit run
   is required (".5" and "E5" are rejected as malformed). */
inline bool isFloat(std::string& str)
{
    size_t i = 0;
    const size_t n = str.size();
    if (n == 0) return false;

    if (str[i] == '+' || str[i] == '-') i++;

    size_t intDigits = 0;
    while (i < n && std::isdigit(static_cast<unsigned char>(str[i]))) { i++; intDigits++; }
    if (intDigits == 0) return false;

    bool hasFraction = false;
    if (i < n && str[i] == '.')
    {
        i++;
        hasFraction = true;
        while (i < n && std::isdigit(static_cast<unsigned char>(str[i]))) i++;
    }

    bool hasExponent = false;
    if (i < n && (str[i] == 'e' || str[i] == 'E'))
    {
        i++;
        hasExponent = true;
        if (i < n && (str[i] == '+' || str[i] == '-')) i++;
        size_t expDigits = 0;
        while (i < n && std::isdigit(static_cast<unsigned char>(str[i]))) { i++; expDigits++; }
        if (expDigits == 0) return false; /* malformed: "1E", "1e-" */
    }

    if (i != n) return false;            /* trailing junk */
    return hasFraction || hasExponent;   /* a bare integer is Decimal, not Float */
}

inline bool isDecimal(std::string& str)
{
    return str.find_first_not_of(decimalF) == std::string::npos;
}

inline bool shouldSkipShortOrWpLine(std::string& line)
{
    return (line.find_first_not_of(wpF) == std::string::npos) || (line.length() < 2);
}

struct Block;
struct Line;

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

    LineItem(Type type, std::string txt, LineItem* next=nullptr)
    {
        _type = type;
        _txt = txt;
        _next = next;
    }

    std::string toText(bool trimmed = true)
    {
        if( _type == Invalid )
            return "Invalid!";

        if (trimmed)
        {
            std::string t = _txt;
            t.erase(std::remove(t.begin(), t.end(), '\"'), t.end());
            return t;
        }
        else
        {
            return _txt;
        }
    };

    uint64_t toUnsigned()
    {
        try {
            switch (_type)
            {
                case String:
                case Identifier:
                    return 0;
                case Hex:
                    return std::stoull(_txt, nullptr, 16);
                case Decimal:
                    return std::stoull(_txt, nullptr, 10);
                case Float:
                    return this->toDouble();
                default: /* Invalid */
                    return 0;
            }
        } catch (...) {
            return 0;
        }
    };

    int64_t toSigned()
    {
        try {
            switch (_type)
            {
                case String:
                case Identifier:
                    return 0;
                case Hex:
                    return std::stoll(_txt, nullptr, 16);
                case Decimal:
                    return std::stoll(_txt, nullptr, 10);
                case Float:
                    return this->toDouble();
                default: /* Invalid */
                    return 0;
            }
        } catch (...) {
            return 0;
        }
    }

    double toDouble()
    {
        try {
            switch (_type)
            {
                case String:
                case Identifier:
                    return 0.0;
                case Hex:
                    return toUnsigned();
                case Decimal:
                case Float:
                    return std::stod(_txt);
                default: /* Invalid */
                    return 0.0;
            }
        } catch (...) {
            return 0.0;
        }
    }

    struct Iterator
    {
        Iterator( LineItem* li ) : _li(li)
        {};

        bool isEnd()
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

        LineItem* li() { return _li; }
      private:
        LineItem* _li;
    };

    LineItem* next() { return _next; };
    Type type() { return _type; };

  private:
    friend Line;
    friend Block;

    LineItem* _next;
    Type _type;
    std::string _txt;
};

struct Line
{
    Line(std::string& line)
    {
        size_t startPos, endPos = 0;

        while ((startPos = line.find_first_not_of(wpF, endPos)) != std::string::npos)
        {
            if (line.at(startPos) == '"')
            {
                size_t closeQuote = line.find_first_of('"', startPos + 1);
                endPos = (closeQuote != std::string::npos) ? closeQuote + 1 : line.size();
            }
            else
            {
                endPos = line.find_first_of(wpF, startPos + 1);
            }

            std::string t = line.substr(startPos, endPos - startPos);

            if (!t.length())
                continue;

            if (isString(t))
            {
                size_t strStartPos = t.find_first_of('\"');
                size_t strEndPos = t.find_last_of('\"');
                if ((strStartPos != std::string::npos) && (strEndPos != std::string::npos))
                {
                    std::string toAdd = t.substr(strStartPos, strEndPos - strStartPos + 1);
                    toAdd.erase(std::remove(toAdd.begin(), toAdd.end(), '\"'), toAdd.end());
                    _line_items.push_back(std::make_unique<LineItem>(LineItem::String, toAdd));
                }
            }
            else if (isHex(t))
            {
                _line_items.push_back(std::make_unique<LineItem>(LineItem::Hex, t));
            }
            else if (isFloat(t))
            {
                _line_items.push_back(std::make_unique<LineItem>(LineItem::Float, t));
            }
            else if (isDecimal(t))
            {
                _line_items.push_back(std::make_unique<LineItem>(LineItem::Decimal, t));
            }
            else
            {
                _line_items.push_back(std::make_unique<LineItem>(LineItem::Identifier, t));
            }
        }
        if( _line_items.size() )
            _line_items.back()->_next = invli(); /* the last item is always linked to the static invalid LineItem */
    }

    static LineItem* invli()
    {
        static LineItem li(LineItem::Invalid, "");
        li._next = &li;
        return &li;
    }

    std::vector<std::unique_ptr<LineItem>> _line_items;
};

struct Block
{
    Block(Block* parent) { _parent = parent; }

    void addChild(std::unique_ptr<Block> b) { _children.push_back(std::move(b)); }

    Block* parent() { return _parent; }

    LineItem* liByIdx(uint64_t idx)
    {
        if (idx < _line_items.size())
            return _line_items.at(idx);
        return Line::invli();
    }

    std::vector<LineItem*> lisByTxtAndType(std::string name, LineItem::Type type)
    {
        std::vector<LineItem*> ret;
        for (auto& li : this->_line_items)
        {
            if ((li->toText() == name) && (li->_type == type))
            {
                ret.push_back(li);
            }
        }
        return ret;
    }

    LineItem* liByIdent(std::string name)
    {
        for (auto& li : this->_line_items)
        {
            if (li->_type == li->Identifier)
            {
                if (li->toText() == name) return li;
            }
        }
        return Line::invli();
    }

    /* Returns the next item (right) to the line item with the name */
    LineItem* liByIdentNxt(std::string name)
    {
        LineItem* li = liByIdent(name);
        return li->_next;
    }

    LineItem* firstLineItem()
    {
        return (this->_line_items.size() ) ? this->_line_items.at(0) : Line::invli();
    }

    Block* childBlockByName(std::string name)
    {
        const auto itr = _children_lookup.find(name);
        return (itr != _children_lookup.end()) ? itr->second : nullptr;
    }

    std::vector<Block*> childBlocksByName(std::string name)
    {
        std::vector<Block*> ret;
        for (auto [itr, rangeEnd] = _children_lookup.equal_range(name); itr != rangeEnd; ++itr)
        {
            ret.push_back(itr->second);
        }
        return ret;
    }

    void parse()
    {
        std::string firstLine;
        std::getline(ss, firstLine);
        parseFirstLineBlkName(firstLine);

        if ( ignoredBlockNames.find(_name) != ignoredBlockNames.end() )
        {
            /* Blacklisted block: skip parsing but preserve raw content. */
            _raw_content = ss.str();
            ss.clear();
            /* Still register in parent lookup so childBlockByName works. */
            if (_parent != nullptr)
            {
                _parent->_children_lookup.insert(std::pair<std::string, Block*>(_name, this));
            }
            return;
        }

        if (!shouldSkipShortOrWpLine(firstLine))
        {
            _lines.push_back(std::make_unique<Line>(firstLine));
        }
        std::string line;
        while (std::getline(ss, line))
        {
            _lines.push_back(std::make_unique<Line>(line));
        }

        /* Register Block in parents map*/
        if (_parent != nullptr)
        {
            _parent->_children_lookup.insert(std::pair<std::string, Block*>(_name, this));
        }

        /* Register line elements in a single list */
        LineItem* lastLi = nullptr;
        for (auto& l : _lines)
        {
            for (auto& li : l->_line_items)
            {
                _line_items.push_back(li.get());
                if (lastLi != nullptr) lastLi->_next = li.get();
                lastLi = li.get();
            }
        }
    }

    std::stringstream ss;
    std::string _name;
    std::string _raw_content; // Raw text for blacklisted blocks (e.g. A2ML).

    Block* _parent;
    std::vector<std::unique_ptr<Block>> _children;
    std::vector<std::unique_ptr<Line>> _lines;

    std::vector<LineItem*> _line_items;
    std::multimap<std::string, Block*> _children_lookup;

  private:
    void parseFirstLineBlkName(std::string& firstLineOfBlock)
    {
        size_t startPos = firstLineOfBlock.find_first_not_of(wpF);
        if (startPos == std::string::npos) return;
        size_t endPos = firstLineOfBlock.find_first_of(wpF, startPos);
        if (endPos == std::string::npos)
            endPos = firstLineOfBlock.size() - 1;
        else
            endPos -= 1;

        _name = std::string(&firstLineOfBlock[startPos], &firstLineOfBlock[endPos + 1]);

        firstLineOfBlock.erase(0, endPos + 1);
    }
};

struct A2lFile
{
    std::unique_ptr<Line> ASAP2_VERSION;
    std::unique_ptr<Block> PROJECT;

    std::pair<uint8_t, uint8_t> version()
    {
        std::pair<uint8_t,uint8_t> ret;
        if( ASAP2_VERSION == nullptr )
            return ret;
        if( ASAP2_VERSION->_line_items.size() != 3 )
            return ret;

        ret.first = ASAP2_VERSION->_line_items[1]->toUnsigned();
        ret.second = ASAP2_VERSION->_line_items[2]->toUnsigned();
        return ret;
    };
};

struct Loader
{
    static std::unique_ptr<A2lFile> readA2lFile(std::string path)
    {
        std::ifstream infile(path);

        if (!infile.is_open())
            return nullptr;

        std::string line;
        uint64_t blockDepth = 0;

        auto af = std::make_unique<A2lFile>();
        std::unique_ptr<Block> mainBlock;
        Block* currentBlock = nullptr;

        uint64_t lineCount = 0;
        bool inBlockComment = false;
        size_t length;
        std::string toAdd;

        while (std::getline(infile, line))
        {
            lineCount++;
            size_t pos = 0;
            size_t endPos = 0;

            /* Strip comments in a single left-to-right pass. A block comment may
               span lines (inBlockComment carries across getline) and reduces to
               one space so it acts as a token separator; a line comment kills the
               rest of the line. Double-quote parity suppresses comment starts
               inside a string (no escape handling, per-line reset - matching the
               tokenizer). */
            {
                std::string stripped;
                stripped.reserve(line.size());
                bool inString = false;
                for (size_t i = 0; i < line.size(); )
                {
                    if (inBlockComment)
                    {
                        size_t e = line.find(commentEnd, i);
                        if (e == std::string::npos) { break; }
                        inBlockComment = false;
                        stripped += ' ';
                        i = e + commentEnd.length();
                    }
                    else if (!inString &&
                             line.compare(i, commentBegin.size(), commentBegin) == 0)
                    {
                        inBlockComment = true;
                        i += commentBegin.length();
                    }
                    else if (!inString &&
                             line.compare(i, lineComment.size(), lineComment) == 0)
                    {
                        break; /* rest of line is a comment */
                    }
                    else
                    {
                        if (line[i] == '"') inString = !inString;
                        stripped += line[i];
                        i++;
                    }
                }
                line = std::move(stripped);
            }

            /* Multi line strings will be merged to a single line for parsing.. */
            if ((pos = line.find_first_of('\"')) != std::string::npos)
            {
                /* there is a string in this line */
                while (pos == line.find_last_of('\"'))
                {
                    std::string t;
                    if (std::getline(infile, t))
                    {
                        lineCount++;
                        line += t;
                    }
                    else
                    {
                        return nullptr;
                    }
                }
            }

            /* Skip short and empty lines */
            if (shouldSkipShortOrWpLine(line))
            {
                continue;
            }

            if (mainBlock == nullptr)
            {
                if ((pos = line.find(asap2ver)) != std::string::npos)
                {
                    af->ASAP2_VERSION = std::make_unique<Line>( line );
                }

                if ((pos = line.find(blockBegin)) != std::string::npos)
                {
                    blockDepth++;
                    mainBlock = std::make_unique<Block>(nullptr);
                    currentBlock = mainBlock.get();
                    pos += blockBegin.length();
                    currentBlock->ss << line.substr(pos, line.size() - pos)
                                     << std::endl;
                }
            }
            else
            {
                if ((pos = line.find(blockBegin)) != std::string::npos)
                {
                    blockDepth++;
                    auto tBlock = std::make_unique<Block>(currentBlock);
                    Block* tBlockRaw = tBlock.get();
                    currentBlock->addChild(std::move(tBlock));
                    currentBlock = tBlockRaw;
                    pos += blockBegin.length();
                    if ((endPos = line.find(blockEnd)) != std::string::npos)
                    {
                        /* Special case -> Block begin and end in the same line.. */
                        length = line.length() - pos - (line.length() - endPos);
                        toAdd = line.substr(pos, length);
                        currentBlock->ss << toAdd << std::endl;
                        blockDepth--;
                        currentBlock->parse();
                        currentBlock = currentBlock->parent();
                    }
                    else
                    {
                        /* Normal case -> only block begin in this line..  */
                        toAdd = line.substr(pos, line.size() - pos);
                        currentBlock->ss << toAdd << std::endl;
                    }
                }
                else if ((pos = line.find(blockEnd)) != std::string::npos)
                {
                    blockDepth--;
                    currentBlock->parse();

                    if (blockDepth == 0)
                    {
                        break; /* finished */
                    }
                    else
                    {
                        currentBlock = currentBlock->parent();

                        pos += blockEnd.length();
                    }
                }
                else
                {
                    currentBlock->ss << line << std::endl;
                }
            }
        }
        af->PROJECT = std::move(mainBlock);
        return af;
    }
};
} // namespace a2lfile

#endif // A2LFILE_H
