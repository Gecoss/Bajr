#pragma once

#include "board.hpp"
#include <unordered_map>

class TTEntry {

    int _value;
    int _depth;
    bool _lower;
    bool _exact;
    bool _upper;
    int _best;

    public:
        TTEntry(int value, unsigned int depth, bool lower, bool exact, bool upper, int best) :
            _value(value),
            _depth(depth),
            _lower(lower),
            _exact(exact),
            _upper(upper),
            _best(best) {}

        TTEntry() : _value(0), _depth(0), _lower(false), _exact(false), _upper(false), _best(0) {}

        bool empty() const {
            return _value == 0 && _depth == 0;
        }

        bool isValid() const {
            return _depth != 0;
        }

        int getBest() const {
            return _best;
        }

        int getValue() const {
            return _value;
        }

        unsigned int getDepth() const {
            return _depth;
        }

        bool isLower() const {
            return _lower;
        }

        bool isExact() const {
            return _exact;
        }

        bool isUpper() const {
            return _upper;
        }

    friend class TranspositionTable;
};

class TranspositionTable {

    std::unordered_map<Board, TTEntry, BoardHash> _entries;

    public:
        TranspositionTable() {
            _entries.reserve(10000000);
        }

        ~TranspositionTable() {}

        void clear() {
            _entries.clear();
        }

        int count(const Board& b) const {
            return _entries.count(b);
        }

        void store(TTEntry entry, const Board& current) {
            if (_entries.count(current) == 0 || _entries[current].getDepth() < entry.getDepth()) {
                _entries[current] = entry;
            }
        }

        const TTEntry& get(const Board& board) {
            auto it = _entries.find(board);
            if (it != _entries.end()) {
                return it->second;
            }
            static TTEntry emptyEntry; // Retornar un objeto TTEntry vacío
            return emptyEntry; 
        }
};

extern TranspositionTable TT;
