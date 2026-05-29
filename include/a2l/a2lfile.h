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
#include <array>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <set>

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

inline const std::string notAllowedInFloatF = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; /* toDo Float might use this notation 12E-2.*/

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

/* toDo 1E-6 etc.. */
inline bool isFloat(std::string& str)
{
    if (str.find('.') == std::string::npos) return false;

    if (str.find_first_of(decimalF) == std::string::npos) return false;

    std::string t;

    for (auto& c : str) t.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));

    if (t.find_first_of(notAllowedInFloatF) != std::string::npos) return false;

    return true;
}

inline bool isDecimal(std::string& str)
{
    return str.find_first_not_of(decimalF) == std::string::npos;
}

inline void printTabs(int count, std::ostream& o = std::cout)
{
    for (auto tabs = 0; tabs < count; tabs++) o << "    ";
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
        Identifier, /* MCD -  not quoted text - ie Keywords */
        Any /* Added for convinience toDo updateFunctions to use it.. */
    };

    static std::string itemTypeToTxt(Type t)
    {
        const std::array<std::string, Identifier + 1> typeNames = {"String", "Decimal", "Hex", "Float", "Identifier"};
        return typeNames[static_cast<int>(t)];
    }

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

    void write(std::ofstream& o)
    {
        if (_type == String)
        {
            o << "\"" << _txt << "\"";
        }
        else
        {
            o << _txt;
        }
    }

    void print()
    {
        std::cout << _txt << "{" << itemTypeToTxt(_type) << "}"
             << "[";
        switch (_type)
        {
            case String:
            case Identifier:
                std::cout << "-";
                break;
            case Hex:
                std::cout << toUnsigned();
                break;
            case Decimal:
                std::cout << toSigned();
                break;
            case Float:
                std::cout << std::stold(_txt);
                break;
            default:
                std::cout << "?";
                break;
        }
        std::cout << "]";
    }


    struct Iterator
    {
        Iterator( LineItem* li ) : _li(li)
        {};

        bool isEnd()
        {
            return ( _li->_next->_type == Invalid );
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

        Iterator& operator+= (int rhs)
        {
            while( rhs )
            {
                _li = _li->_next;
                rhs--;
            }
            return *this;
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
    Line(std::string& line, bool wasOnBlockBegin = false) : wasOnBlockBegin(wasOnBlockBegin)
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

    void write(std::ofstream& o)
    {
        auto pos = _line_items.begin();

        while (pos != _line_items.end())
        {
            (*pos)->write(o);
            ++pos;
            if (pos != _line_items.end()) o << " ";
        }
    }

    void print()
    {
        auto pos = _line_items.begin();

        while (pos != _line_items.end())
        {
            (*pos)->print();
            ++pos;
            if (pos != _line_items.end()) std::cout << " ";
        }
    }

    static LineItem* invli()
    {
        static LineItem li(LineItem::Invalid, "");
        li._next = &li;
        return &li;
    }

    bool wasOnBlockBegin;
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

    std::vector<LineItem*> lisByTxts(std::set<std::string>& s)
    {
        std::vector<LineItem*> lis;
        for (const auto& li : this->_line_items)
        {
            if ( s.find(li->toText() ) != s.end() )
            {
                lis.push_back( li );
            }
        }
        return lis;
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

    std::vector<LineItem*> lisByTxtsAndType( std::set<std::string>& s, LineItem::Type type)
    {
        std::vector<LineItem*> ret;
        for (const auto& li : this->_line_items)
        {
            if ( ( s.find(li->toText() ) != s.end() ) && (li->_type == type) )
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

    std::vector<Block*> childBlocksByNames(std::vector<std::string> names)
    {
        std::vector<Block*> ret;
        for (const auto& name : names)
        {
            std::vector<Block*> t = childBlocksByName(name);
            ret.insert(ret.end(), t.begin(), t.end());
        }
        return ret;
    }

    void print(int depth = 0)
    {
        printTabs(depth);
        std::cout << _name << "{BLOCK_BEGIN}" << std::endl;
        for (auto& l : _lines)
        {
            printTabs(depth + 1);
            l->print();
            std::cout << std::endl;
        }
        for (auto& c : _children)
        {
            c->print(depth + 1);
        }
        printTabs(depth);
        std::cout << _name << "{BLOCK_END}" << std::endl;
    }

    void write(int depth, std::ofstream& o)
    {
        o << "/begin " << _name << " ";
        for (auto& l : _lines)
        {
            if (!l->wasOnBlockBegin)
            {
                o << std::endl;
                printTabs(depth + 1, o);
            }
            l->write(o);
        }
        o << std::endl;
        for (auto& c : _children)
        {
            c->write(depth + 1, o);
        }
        o << "/end " << _name << std::endl << std::endl;
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
            _lines.push_back(std::make_unique<Line>(firstLine, true));
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
    bool parseFirstLineBlkName(std::string& firstLineOfBlock)
    {
        size_t startPos = firstLineOfBlock.find_first_not_of(wpF);
        if (startPos == std::string::npos) return false;
        size_t endPos = firstLineOfBlock.find_first_of(wpF, startPos);
        if (endPos == std::string::npos)
            endPos = firstLineOfBlock.size() - 1;
        else
            endPos -= 1;

        _name = std::string(&firstLineOfBlock[startPos], &firstLineOfBlock[endPos + 1]);

        firstLineOfBlock.erase(0, endPos + 1);
        return true;
    }
};

struct A2lFile
{
    std::unique_ptr<Line> ASAP2_VERSION;
    std::unique_ptr<Block> PROJECT;

    void print()
    {
        PROJECT->print();
    }
    void write(std::ofstream& o)
    {
        PROJECT->write(0, o);
    }

    struct Version {
        uint8_t mainVer;
        uint8_t subVer;
    };

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

struct Writer
{
    static bool writeA2lFile(std::string path, A2lFile* file)
    {
        if (file->PROJECT == nullptr)
            return false;

        std::ofstream ofile(path);
        if (!ofile.is_open())
            return false;

        file->write(ofile);
        ofile.flush();

        bool ok = ofile.good();
        ofile.close();
        return ok;
    }
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
        size_t length; /* could be removed - debug only */
        std::string toAdd;  /* could be removed - debug only */

        while (std::getline(infile, line))
        {
            lineCount++;
            size_t pos = 0;
            size_t endPos = 0;

            /* Purge comments on the fly: block comments first, then line comments */
            if ((pos = line.find(commentBegin)) != std::string::npos)
            {
                if ((endPos = line.find(commentEnd)) != std::string::npos)
                {
                    /* single line comment */
                    length = endPos + commentEnd.length() - pos;
                    line.erase(pos, length);
                }
                else
                {
                    /* in multi line comment */
                    while (std::getline(infile, line))
                    {
                        lineCount++;
                        if ((pos = line.find_first_not_of(wpF)) != std::string::npos)
                        {
                            if ((endPos = line.find(commentEnd)) != std::string::npos)
                            {
                                /* comment end found */
                                break;
                            }
                            else
                            {
                                /* still in multi line comment */
                                continue;
                            }
                        }
                    }
                    continue;
                }
            }

            /* Strip inline // comments (after block comments are handled).
               Only strip if // is not inside a quoted string. */
            {
                size_t lcPos = line.find(lineComment);
                if (lcPos != std::string::npos)
                {
                    size_t quotesBefore = 0;
                    for (size_t i = 0; i < lcPos; i++)
                        if (line[i] == '"') quotesBefore++;

                    if (quotesBefore % 2 == 0)
                    {
                        line.erase(lcPos);
                        if (line.find_first_not_of(wpF) == std::string::npos)
                            continue;
                    }
                }
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
                        toAdd = line.substr(pos, line.size() - pos);
                        /* Nothing to add after end */;
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
