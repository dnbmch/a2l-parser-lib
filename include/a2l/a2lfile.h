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

/*
 *
 *
 */

namespace a2lfile
{

using namespace std;

/* ToDo move them to a class */
const string asap2ver = "ASAP2_VERSION";
const string blockBegin = "/begin";
const string blockEnd = "/end";
const unordered_set<string> ignoredBlockNames = {"A2ML"};
const string commentBegin = "/*";
const string commentEnd = "*/";
const string lineComment = "//";

const vector<char> decimalChars = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '-'}; /* dot . is not included */
const string decimalF = string(decimalChars.begin(), decimalChars.end());

const string notAllowedInFloatF = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; /* toDo Float might use this notation 12E-2.*/

const string alphaF = "ABCDEFGHIJKLMNOPQRSTUVWXYZ_-'+[]{}()"; /* UNUSED!! */

const vector<char> canBeEscaped = {'\'', '"', 'n', 'r', 't'}; /* UNUSED!! toDo string allow to escape chk trash!! */

const vector<char> wpChars = {0x09 /*HT*/, 0x0A /*LF/NL*/, 0x0B /*VT*/, 0x0C /*FF/NP*/, 0x0D /*CR*/, 0x20 /*SPC*/};
const string wpF = string(wpChars.begin(), wpChars.end());

static bool isString(string& str)
{
    if (str.find("\"") != string::npos) /* toDo can be written in one line.. */
        return true;
    else
        return false;
}

static bool isHex(string& str)
{
    if (str.find("0x") != string::npos) /* toDo can be written in one line.. */
        return true;
    return false;
}

/* toDo 1E-6 etc.. */
static bool isFloat(string& str)
{
    if (str.find('.') == string::npos) return false;

    if (str.find_first_of(decimalF) == string::npos) return false;

    string t;

    for (auto& c : str) t.push_back(toupper(c));

    if (t.find_first_of(notAllowedInFloatF) != string::npos) return false;

    return true;
}

static bool isDecimal(string& str)
{
    if (str.find_first_not_of(decimalF) == string::npos)
        return true;
    else
        return false;
    // return all_of(str.begin(), str.end(), ::isdigit);
}

static void printTabs(int count, ostream& o = cout)
{
    for (auto tabs = 0; tabs < count; tabs++) o << "    ";
}

static bool shouldSkipShortOrWpLine(string& line)
{
    if ((line.find_first_not_of(wpF) == string::npos) || (line.length() < 2))
    {
        return true;
    }
    return false;
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

    static string itemTypeToTxt(Type t)
    {
        const array<string, Identifier + 1> typeNames = {"String", "Decimal", "Hex", "Float", "Identifier"};
        return typeNames[static_cast<int>(t)];
    }

    LineItem(Type type, string txt, LineItem* next=nullptr)
    {
        _type = type;
        _txt = txt;
        _next = next;
    }

    string toText(bool trimmed = true)
    {
        if( _type == Invalid )
            return "Invalid!";

        if (trimmed)
        {
            string t = _txt;
            t.erase(remove(t.begin(), t.end(), '\"'), t.end());
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
                    return stoull(_txt, nullptr, 16);
                case Decimal:
                    return stoull(_txt, nullptr, 10);
                case Float:
                    return this->toDouble();
                default: /* Invalid */
                    return 0;
            }
        } catch (...) {
            cerr << "Warning: malformed token '" << _txt << "'" << endl;
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
                    return stoll(_txt, nullptr, 16);
                case Decimal:
                    return stoll(_txt, nullptr, 10);
                case Float:
                    return this->toDouble();
                default: /* Invalid */
                    return 0;
            }
        } catch (...) {
            cerr << "Warning: malformed token '" << _txt << "'" << endl;
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
                    return stod(_txt);
                default: /* Invalid */
                    return 0.0;
            }
        } catch (...) {
            cerr << "Warning: malformed token '" << _txt << "'" << endl;
            return 0.0;
        }
    }

    void write(ofstream& o)
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
        cout << _txt << "{" << itemTypeToTxt(_type) << "}"
             << "[";
        switch (_type)
        {
            case String:
            case Identifier:
                cout << "-";
                break;
            case Hex:
                cout << toUnsigned();
                break;
            case Decimal:
                cout << toSigned();
                break;
            case Float:
                cout << stold(_txt);
                break;
            default:
                cout << "?";
                break;
        }
        cout << "]";
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
    string _txt;
};

struct Line
{
    Line(string& line, bool wasOnBlockBegin = false) : wasOnBlockBegin(wasOnBlockBegin)
    {
        size_t startPos, endPos = 0;

        while ((startPos = line.find_first_not_of(wpF, endPos)) != string::npos)
        {
            if (line.at(startPos) == '"')
            {
                size_t closeQuote = line.find_first_of('"', startPos + 1);
                endPos = (closeQuote != string::npos) ? closeQuote + 1 : line.size();
            }
            else
            {
                endPos = line.find_first_of(wpF, startPos + 1);
            }

            string t = line.substr(startPos, endPos - startPos);

            if (!t.length())
                continue;

            if (isString(t))
            {
                size_t strStartPos = t.find_first_of('\"');
                size_t strEndPos = t.find_last_of('\"');
                if ((strStartPos == string::npos) || (strEndPos == string::npos))
                    cout << "Not a valid string:" << t << endl;
                else
                {
                    string toAdd = t.substr(strStartPos, strEndPos - strStartPos + 1);
                    toAdd.erase(remove(toAdd.begin(), toAdd.end(), '\"'), toAdd.end());
                    _lineItems.push_back(make_unique<LineItem>(LineItem::String, toAdd));
                }
            }
            else if (isHex(t))
            {
                _lineItems.push_back(make_unique<LineItem>(LineItem::Hex, t));
            }
            else if (isFloat(t))
            {
                _lineItems.push_back(make_unique<LineItem>(LineItem::Float, t));
            }
            else if (isDecimal(t))
            {
                _lineItems.push_back(make_unique<LineItem>(LineItem::Decimal, t));
            }
            else
            {
                _lineItems.push_back(make_unique<LineItem>(LineItem::Identifier, t));
            }
        }
        if( _lineItems.size() )
            _lineItems.back()->_next = invli(); /* the last item is always linked to the static invalid LineItem */
    }

    void write(ofstream& o)
    {
        auto pos = _lineItems.begin();

        while (pos != _lineItems.end())
        {
            (*pos)->write(o);
            ++pos;
            if (pos != _lineItems.end()) o << " ";
        }
    }

    void print()
    {
        auto pos = _lineItems.begin();

        while (pos != _lineItems.end())
        {
            (*pos)->print();
            ++pos;
            if (pos != _lineItems.end()) cout << " ";
        }
    }

    static LineItem* invli()
    {
        static LineItem li(LineItem::Invalid, "");
        li._next = &li;
        return &li;
    }

    bool wasOnBlockBegin;
    vector<unique_ptr<LineItem>> _lineItems;
};

struct Block
{
    Block(Block* parent) { _parent = parent; }

    void addChild(unique_ptr<Block> b) { _children.push_back(std::move(b)); }

    Block* parent() { return _parent; }

    LineItem* liByIdx(uint64_t idx)
    {
        if (idx < _lineItems.size())
            return _lineItems.at(idx);
        return Line::invli();
    }

    std::vector<LineItem*> lisByTxts(set<string>& s)
    {
        std::vector<LineItem*> lis;
        for (const auto& li : this->_lineItems)
        {
            if ( s.find(li->toText() ) != s.end() )
            {
                lis.push_back( li );
            }
        }
        return lis;
    }

    vector<LineItem*> lisByTxtAndType(string name, LineItem::Type type)
    {
        vector<LineItem*> ret;
        for (auto& li : this->_lineItems)
        {
            if ((li->toText() == name) && (li->_type == type))
            {
                ret.push_back(li);
            }
        }
        return ret;
    }

    vector<LineItem*> lisByTxtsAndType( set<string>& s, LineItem::Type type)
    {
        vector<LineItem*> ret;
        for (const auto& li : this->_lineItems)
        {
            if ( ( s.find(li->toText() ) != s.end() ) && (li->_type == type) )
            {
                ret.push_back(li);
            }
        }
        return ret;
    }

    LineItem* liByIdent(string name)
    {
        for (auto& li : this->_lineItems)
        {
            if (li->_type == li->Identifier)
            {
                if (li->toText() == name) return li;
            }
        }
        return Line::invli();
    }

    /* Returns the next item (right) to the line item with the name */
    LineItem* liByIdentNxt(string name)
    {
        LineItem* li = liByIdent(name);
        return li->_next;
    }

    LineItem* firstLineItem()
    {
        return (this->_lineItems.size() ) ? this->_lineItems.at(0) : Line::invli();
    }

    Block* childBlockByName(string name)
    {
        const auto itr = _childrenLookup.find(name);
        return (itr != _childrenLookup.end()) ? itr->second : nullptr;
    }

    vector<Block*> childBlocksByName(string name)
    {
        vector<Block*> ret;
        for (auto [itr, rangeEnd] = _childrenLookup.equal_range(name); itr != rangeEnd; ++itr)
        {
            ret.push_back(itr->second);
        }
        return ret;
    }

    vector<Block*> childBlocksByNames(vector<string> names)
    {
        vector<Block*> ret;
        for (const auto& name : names)
        {
            vector<Block*> t = childBlocksByName(name);
            ret.insert(ret.end(), t.begin(), t.end());
        }
        return ret;
    }

    void print(int depth = 0)
    {
        printTabs(depth);
        cout << _name << "{BLOCK_BEGIN}" << endl;
        for (auto& l : _lines)
        {
            printTabs(depth + 1);
            l->print();
            cout << endl;
        }
        for (auto& c : _children)
        {
            c->print(depth + 1);
        }
        printTabs(depth);
        cout << _name << "{BLOCK_END}" << endl;
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
        o << endl;
        for (auto& c : _children)
        {
            c->write(depth + 1, o);
        }
        o << "/end " << _name << endl << endl;
    }

    void parse()
    {
        string firstLine;
        getline(ss, firstLine);
        parseFirstLineBlkName(firstLine);

        if ( ignoredBlockNames.find(_name) != ignoredBlockNames.end() )
        {
            /* Blacklisted block: skip parsing but preserve raw content. */
            _rawContent = ss.str();
            ss.clear();
            /* Still register in parent lookup so childBlockByName works. */
            if (_parent != nullptr)
            {
                _parent->_childrenLookup.insert(pair<string, Block*>(_name, this));
            }
            return;
        }

        if (!shouldSkipShortOrWpLine(firstLine))
        {
            _lines.push_back(make_unique<Line>(firstLine, true));
        }
        string line;
        while (getline(ss, line))
        {
            _lines.push_back(make_unique<Line>(line));
        }

        /* Register Block in parents map*/
        if (_parent != nullptr)
        {
            _parent->_childrenLookup.insert(pair<string, Block*>(_name, this));
        }

        /* Register line elements in a single list */
        LineItem* lastLi = nullptr;
        for (auto& l : _lines)
        {
            for (auto& li : l->_lineItems)
            {
                _lineItems.push_back(li.get());
                if (lastLi != nullptr) lastLi->_next = li.get();
                lastLi = li.get();
            }
        }
    }

    stringstream ss;
    string _name;
    string _rawContent; // Raw text for blacklisted blocks (e.g. A2ML).

    Block* _parent;
    vector<unique_ptr<Block>> _children;
    vector<unique_ptr<Line>> _lines;

    vector<LineItem*> _lineItems;
    multimap<string, Block*> _childrenLookup;

  private:
    bool parseFirstLineBlkName(string& firstLineOfBlock)
    {
        size_t startPos = firstLineOfBlock.find_first_not_of(wpF);
        if (startPos == string::npos) return false;
        size_t endPos = firstLineOfBlock.find_first_of(wpF, startPos);
        if (endPos == string::npos)
            endPos = firstLineOfBlock.size() - 1;
        else
            endPos -= 1;

        _name = string(&firstLineOfBlock[startPos], &firstLineOfBlock[endPos + 1]);

        firstLineOfBlock.erase(0, endPos + 1);
        return true;
    }
};

struct A2lFile
{
    unique_ptr<Line> ASAP2_VERSION;
    unique_ptr<Block> PROJECT;

    void print()
    {
        PROJECT->print();
    }
    void write(ofstream& o)
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
        if( ASAP2_VERSION->_lineItems.size() != 3 )
            return ret;

        ret.first = ASAP2_VERSION->_lineItems[1]->toUnsigned();
        ret.second = ASAP2_VERSION->_lineItems[2]->toUnsigned();
        return ret;
    };
};

struct Writer
{
    static bool writeA2lFile(string path, A2lFile* file)
    {
        if (file->PROJECT == nullptr)
            return false;

        ofstream ofile(path);
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
    static unique_ptr<A2lFile> readA2lFile(string path)
    {
        ifstream infile(path);

        if (!infile.is_open())
            return nullptr;

        string line;
        uint64_t blockDepth = 0;

        auto af = make_unique<A2lFile>();
        unique_ptr<Block> mainBlock;
        Block* currentBlock = nullptr;

        uint64_t lineCount = 0;
        size_t length; /* could be removed - debug only */
        string toAdd;  /* could be removed - debug only */

        while (getline(infile, line))
        {
            lineCount++;
            size_t pos = 0;
            size_t endPos = 0;

            /* Purge comments on the fly: block comments first, then line comments */
            if ((pos = line.find(commentBegin)) != string::npos)
            {
                if ((endPos = line.find(commentEnd)) != string::npos)
                {
                    /* single line comment */
                    length = endPos + commentEnd.length() - pos;
                    line.erase(pos, length);
                }
                else
                {
                    /* in multi line comment */
                    while (getline(infile, line))
                    {
                        lineCount++;
                        if ((pos = line.find_first_not_of(wpF)) != string::npos)
                        {
                            if ((endPos = line.find(commentEnd)) != string::npos)
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
                if (lcPos != string::npos)
                {
                    size_t quotesBefore = 0;
                    for (size_t i = 0; i < lcPos; i++)
                        if (line[i] == '"') quotesBefore++;

                    if (quotesBefore % 2 == 0)
                    {
                        line.erase(lcPos);
                        if (line.find_first_not_of(wpF) == string::npos)
                            continue;
                    }
                }
            }

            /* Multi line strings will be merged to a single line for parsing.. */
            if ((pos = line.find_first_of('\"')) != string::npos)
            {
                /* there is a string in this line */
                while (pos == line.find_last_of('\"'))
                {
                    string t;
                    if (getline(infile, t))
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
                if ((pos = line.find(asap2ver)) != string::npos)
                {
                    af->ASAP2_VERSION = make_unique<Line>( line );
                }

                if ((pos = line.find(blockBegin)) != string::npos)
                {
                    blockDepth++;
                    mainBlock = make_unique<Block>(nullptr);
                    currentBlock = mainBlock.get();
                    pos += blockBegin.length();
                    currentBlock->ss << line.substr(pos, line.size() - pos)
                                     << endl;
                }
            }
            else
            {
                if ((pos = line.find(blockBegin)) != string::npos)
                {
                    blockDepth++;
                    auto tBlock = make_unique<Block>(currentBlock);
                    Block* tBlockRaw = tBlock.get();
                    currentBlock->addChild(std::move(tBlock));
                    currentBlock = tBlockRaw;
                    pos += blockBegin.length();
                    if ((endPos = line.find(blockEnd)) != string::npos)
                    {
                        /* Special case -> Block begin and end in the same line.. */
                        length = line.length() - pos - (line.length() - endPos);
                        toAdd = line.substr(pos, length);
                        currentBlock->ss << toAdd << endl;
                        blockDepth--;
                        currentBlock->parse();
                        currentBlock = currentBlock->parent();
                    }
                    else
                    {
                        /* Normal case -> only block begin in this line..  */
                        toAdd = line.substr(pos, line.size() - pos);
                        currentBlock->ss << toAdd << endl;
                    }
                }
                else if ((pos = line.find(blockEnd)) != string::npos)
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
                    currentBlock->ss << line << endl;
                }
            }
        }
        cout << "Finished with lineCount: " << lineCount << endl;
        af->PROJECT = std::move(mainBlock);
        return af;
    }
};
} // namespace a2lfile

#endif // A2LFILE_H
