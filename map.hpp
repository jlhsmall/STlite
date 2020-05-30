/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include<iostream>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
    /**
     * the internal type of data.
    * it should have a default constructor, a copy constructor.
    * You can use sjtu::map as value_type by typedef.
    */
    typedef pair<const Key, T> value_type;

    /**
     * see BidirectionalIterator at CppReference for help.
     *
     * if there is anything wrong throw invalid_iterator.
     *     like it = map.begin(); --it;
     *       or it = map.end(); ++end();
     */
    struct node {
        map *belong;
        value_type *val;
        bool col;//RED:0,Black:1
        node *lc, *rc, *pre;

        node(map *_belong) : belong(_belong) {
            val = NULL;
            lc = rc = pre = NULL;
            col = 0;
        }

        node(map *_belong, value_type _val, int _col = 0) : belong(_belong), col(_col) {
            val = new value_type(_val);
            lc = rc = pre = NULL;
        }

        node(const node &other) {
            belong = other.belong;
            col = other.col;
            if (other.val != NULL)val = new value_type(*other.val);
            else val = NULL;
            lc = rc = pre = NULL;
        }

        ~node() {
            delete val;
        }

        bool operator<(const node &right) const {
            if (val == NULL)return 0;
            if (right.val == NULL)return 1;
            return Compare()(val->first, right.val->first);
        }

        node *getnxt() {
            node *x = this;
            if (x->rc != NULL) {
                for (x = x->rc; x->lc != NULL; x = x->lc);
                return x;
            }
            while (x->pre != NULL && x->pre->lc != x)x = x->pre;
            if (x->pre == NULL)throw invalid_iterator();
            return x->pre;
        }

        node *getpre() {
            node *x = this;
            if (x->lc != NULL) {
                for (x = x->lc; x->rc != NULL; x = x->rc);
                return x;
            }
            while (x->pre != NULL && x->pre->rc != x)x = x->pre;
            if (x->pre == NULL)throw invalid_iterator();
            return x->pre;
        }
    };

    node *root, *ed;
    int sz;

    class const_iterator;

    class iterator {
    private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
    public:
        node *cur;

        bool check_invalid() const { return cur == NULL; }

        bool check_end() const { return cur->val == NULL; }


        iterator(node *_cur = NULL) : cur(_cur) {}

        iterator(const iterator &other) {
            cur = other.cur;
        }
        /**
         * return a new iterator which pointer n-next elements
         *   even if there are not enough elements, just return the answer.
         * as well as operator-
         */
        /**
         * TODO iter++
         */
        iterator operator++(int) {
            if (check_invalid())throw invalid_iterator();
            iterator ret(*this);
            cur = cur->getnxt();
            return ret;
        }

        /**
         * TODO ++iter
         */
        iterator &operator++() {
            if (check_invalid())throw invalid_iterator();
            cur = cur->getnxt();
            return *this;
        }

        /**
         * TODO iter--
         */
        iterator operator--(int) {
            if (check_invalid())throw invalid_iterator();
            iterator ret(*this);
            cur = cur->getpre();
            return ret;
        }

        /**
         * TODO --iter
         */
        iterator &operator--() {
            if (check_invalid())throw invalid_iterator();
            cur = cur->getpre();
            return *this;
        }

        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        value_type &operator*() const {
            if (check_invalid() || check_end())throw invalid_iterator();
            return *cur->val;
        }

        bool operator==(const iterator &rhs) const { return cur == rhs.cur; }

        bool operator==(const const_iterator &rhs) const { return cur == rhs.cur; }

        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const { return cur != rhs.cur; }

        bool operator!=(const const_iterator &rhs) const { return cur != rhs.cur; }

        /**
         * for the support of it->first.
         * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
         */
        value_type *operator->() const noexcept {
            //if(check_invalid()||check_end())throw invalid_iterator();
            return cur->val;
        }
    };

    class const_iterator {
        // it should has similar member method as iterator.
        //  and it should be able to construct from an iterator.
    private:
        // data members.
    public:
        node *cur;

        bool check_invalid() const { return cur == NULL; }

        bool check_end() const { return cur->val == NULL; }

        const_iterator(node *_cur = NULL) : cur(_cur) {}

        const_iterator(const const_iterator &other) {
            cur = other.cur;
        }

        const_iterator(const iterator &other) {
            cur = other.cur;
        }

        const_iterator operator++(int) {
            if (check_invalid())throw invalid_iterator();
            const_iterator ret(*this);
            cur = cur->getnxt();
            return ret;
        }

        /**
         * TODO ++iter
         */
        const_iterator &operator++() {
            if (check_invalid())throw invalid_iterator();
            cur = cur->getnxt();
            return *this;
        }

        /**
         * TODO iter--
         */
        const_iterator operator--(int) {
            if (check_invalid())throw invalid_iterator();
            const_iterator ret(*this);
            cur = cur->getpre();
            return ret;
        }

        /**
         * TODO --iter
         */
        const_iterator &operator--() {
            if (check_invalid())throw invalid_iterator();
            cur = cur->getpre();
            return *this;
        }

        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        const value_type &operator*() const {
            if (check_invalid() || check_end())throw invalid_iterator();
            return *cur->val;
        }

        bool operator==(const const_iterator &rhs) const { return cur == rhs.cur; }

        bool operator==(const iterator &rhs) const { return cur == rhs.cur; }

        /**
         * some other operator for iterator.
         */
        bool operator!=(const const_iterator &rhs) const { return cur != rhs.cur; }

        bool operator!=(const iterator &rhs) const { return cur != rhs.cur; }

        const value_type *operator->() const noexcept {
            //if(check_invalid()||check_end())throw invalid_iterator();
            return cur->val;
        }
    };

    /**
     * TODO two constructors
     */
    map() {
        ed = root = new node(this);
        sz = 0;
    }

    node *copy(node *y) {
        node *x = new node(*y);
        x->belong = this;
        if (y->lc != NULL) {
            x->lc = copy(y->lc);
            x->lc->pre = x;
        }
        if (y->rc != NULL) {
            x->rc = copy(y->rc);
            x->rc->pre = x;
        }
        return x;
    }

    map(const map &other) {
        root = copy(other.root);
        for (ed = root; ed->rc != NULL; ed = ed->rc);
        sz = other.sz;
    }

    /**
     * TODO assignment operator
     */
    void del(node *x) {
        if (x->lc != NULL)del(x->lc);
        if (x->rc != NULL)del(x->rc);
        delete x;
    }

    map &operator=(const map &other) {
        if (this == &other)return *this;
        if (root != NULL)del(root);
        root = copy(other.root);
        for (ed = root; ed->rc != NULL; ed = ed->rc);
        sz = other.sz;
        return *this;
    }

    /**
     * TODO Destructors
     */
    ~map() {
        del(root);
    }

    /**
     * TODO
     * access specified element with bounds checking
     * Returns a reference to the mapped value of the element with key equivalent to key.
     * If no such element exists, an exception of type `index_out_of_bound'
     */
    void LL(node *x) {
        node *y = x->lc;
        x->lc = y->rc;
        if (y->rc != NULL)y->rc->pre = x;
        y->rc = x;
        y->pre = x->pre;
        x->pre = y;
        if (root == x)root = y;
        else if (y->pre->lc == x)y->pre->lc = y;
        else y->pre->rc = y;
    }

    void RR(node *x) {
        node *y = x->rc;
        x->rc = y->lc;
        if (y->lc != NULL)y->lc->pre = x;
        y->lc = x;
        y->pre = x->pre;
        x->pre = y;
        if (root == x)root = y;
        else if (y->pre->lc == x)y->pre->lc = y;
        else y->pre->rc = y;
    }

    void LR(node *x) {
        RR(x->lc);
        LL(x);
    }

    void RL(node *x) {
        LL(x->rc);
        RR(x);
    }

    void insert_rebalance(node *x) {
        node *fa = x->pre, *gr, *un;
        while (fa != NULL && !fa->col) {
            if (fa == root) {
                fa->col = 1;
                return;
            }
            gr = fa->pre;
            un = gr->lc == fa ? gr->rc : gr->lc;
            if (un == NULL || un->col == 1) {
                if (fa == gr->lc) {
                    if (x == fa->lc) {
                        gr->col = 0;
                        fa->col = 1;
                        LL(gr);
                    } else {
                        gr->col = 0;
                        x->col = 1;
                        LR(gr);
                    }
                } else {
                    if (x == fa->lc) {
                        gr->col = 0;
                        x->col = 1;
                        RL(gr);
                    } else {
                        gr->col = 0;
                        fa->col = 1;
                        RR(gr);
                    }
                }
                return;
            }
            gr->col = 0;
            fa->col = un->col = 1;
            x = gr;
            fa = x->pre;
        }
    }

    void erase_rebalance(node *x) {
        node *fa = x->pre, *br;
        while (fa != NULL) {
            br = fa->lc == x ? fa->rc : fa->lc;
            if (!br->col) {
                fa->col = 0;
                br->col = 1;
                if (fa->lc == br)LL(fa); else RR(fa);
            } else {
                if ((br->lc == NULL || br->lc->col == 1) && (br->rc == NULL || br->rc->col == 1)) {
                    br->col = 0;
                    if (!fa->col) {
                        fa->col = 1;
                        return;
                    }
                    x = fa;
                    fa = x->pre;
                } else {
                    if (fa->lc == br) {
                        if (br->rc != NULL && !br->rc->col) {
                            br->rc->col = fa->col;
                            fa->col = 1;
                            LR(fa);
                        } else {
                            br->col = fa->col;
                            br->lc->col = fa->col = 1;
                            LL(fa);
                        }
                    } else {
                        if (br->lc != NULL && !br->lc->col) {
                            br->lc->col = fa->col;
                            fa->col = 1;
                            RL(fa);
                        } else {
                            br->col = fa->col;
                            br->rc->col = fa->col = 1;
                            RR(fa);
                        }
                    }
                    return;
                }
            }
        }
    }

    T &at(const Key &key) {
        node *x = root;
        while (x != NULL) {
            if (x->val != NULL && !Compare()(key, x->val->first) && !Compare()(x->val->first, key))
                return x->val->second;
            x = x->val == NULL || Compare()(key, x->val->first) ? x->lc : x->rc;
        }
        throw index_out_of_bound();
    }

    const T &at(const Key &key) const {
        node *x = root;
        while (x != NULL) {
            if (x->val != NULL && !Compare()(key, x->val->first) && !Compare()(x->val->first, key))
                return x->val->second;
            x = x->val == NULL || Compare()(key, x->val->first) ? x->lc : x->rc;
        }
        throw index_out_of_bound();
    }

    /**
     * TODO
     * access specified element
     * Returns a reference to the value that is mapped to a key equivalent to key,
     *   performing an insertion if such key does not already exist.
     */
    T &operator[](const Key &key) {
        node *x = root;
        while (x->val == NULL || Compare()(key, x->val->first) || Compare()(x->val->first, key)) {
            //std::cout<<1;
            node *&v = x->val == NULL || Compare()(key, x->val->first) ? x->lc : x->rc;
            if (v == NULL) {
                ++sz;//std::cout<<std::endl;
                v = new node(this, value_type(key, T()));
                v->pre = x;
                node *y = v;
                if (!x->col)insert_rebalance(v);
                return y->val->second;
            }
            x = v;
        }
        return x->val->second;
    }

    /**
     * behave like at() throw index_out_of_bound if such key does not exist.
     */
    const T &operator[](const Key &key) const {
        node *x = root;
        while (x != NULL) {
            if (x->val != NULL && !Compare()(key, x->val->first) && !Compare()(x->val->first, key))
                return x->val->second;
            x = x->val == NULL || Compare()(key, x->val->first) ? x->lc : x->rc;
        }
        throw index_out_of_bound();
    }

    /**
     * return a iterator to the beginning
     */
    iterator begin() {
        node *x = root;
        while (x->lc != NULL)x = x->lc;
        return iterator(x);
    }

    const_iterator cbegin() const {
        node *x = root;
        while (x->lc != NULL)x = x->lc;
        return const_iterator(x);
    }

    /**
     * return a iterator to the end
     * in fact, it returns
     */
    iterator end() {
        return iterator(ed);
    }

    const_iterator cend() const {
        return const_iterator(ed);
    }

    /**
     * checks whether the container is empty
     * return true if empty, otherwise false.
     */
    bool empty() const { return !sz; }

    /**
     * returns the number of elements.
     */
    size_t size() const { return sz; }

    /**
     * clears the contents
     */
    void clear() {
        del(root);
        ed = root = new node(this);
        sz = 0;
    }

    /**
     * insert an element.
     * return a pair, the first of the pair is
     *   the iterator to the new element (or the element that prevented the insertion),
     *   the second one is true if insert successfully, or false.
     */
    pair<iterator, bool> insert(const value_type &value) {
        node *x = root;
        while (x->val == NULL || Compare()(value.first, x->val->first) || Compare()(x->val->first, value.first)) {
            node *&v = x->val == NULL || Compare()(value.first, x->val->first) ? x->lc : x->rc;
            if (v == NULL) {
                ++sz;
                v = new node(this, value);
                v->pre = x;
                node *y = v;
                if (!x->col)insert_rebalance(v);
                return pair<iterator, bool>(y, true);
            }
            x = v;
        }
        return pair<iterator, bool>(iterator(x), false);
    }

    /**
     * erase the element at pos.
     *
     * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
     */
    void SWAP(node *x, node *y) {
        if (x->pre == y)std::swap(x, y);
        if (x->lc == y) {
            if (x->rc != NULL)x->rc->pre = y;
            if (y->lc != NULL)y->lc->pre = x;
            if (y->rc != NULL)y->rc->pre = x;
            if (x->pre != NULL) {
                if (x->pre->lc == x)x->pre->lc = y;
                else x->pre->rc = y;
            } else root = y;
            y->pre = x->pre;
            x->pre = y;
            x->lc = y->lc;
            y->lc = x;
            std::swap(x->rc, y->rc);
            std::swap(x->col, y->col);
        } else if (x->rc == y) {
            if (x->lc != NULL)x->lc->pre = y;
            if (y->lc != NULL)y->lc->pre = x;
            if (y->rc != NULL)y->rc->pre = x;
            if (x->pre != NULL) {
                if (x->pre->lc == x)x->pre->lc = y;
                else x->pre->rc = y;
            } else root = y;
            y->pre = x->pre;
            x->pre = y;
            x->rc = y->rc;
            y->rc = x;
            std::swap(x->lc, y->lc);
            std::swap(x->col, y->col);
        } else {
            if (x->lc != NULL)x->lc->pre = y;
            if (x->rc != NULL)x->rc->pre = y;
            if (y->lc != NULL)y->lc->pre = x;
            if (y->rc != NULL)y->rc->pre = x;
            if (x->pre != NULL) {
                if (x->pre->lc == x)x->pre->lc = y;
                else x->pre->rc = y;
            } else root = y;
            if (y->pre != NULL) {
                if (y->pre->lc == y)y->pre->lc = x;
                else y->pre->rc = x;
            } else root = x;
            std::swap(x->lc, y->lc);
            std::swap(x->rc, y->rc);
            std::swap(x->pre, y->pre);
            std::swap(x->col, y->col);
        }
    }

    void erase(iterator pos) {
        if (pos.cur->belong != this)throw invalid_iterator();
        if (pos.check_invalid() || pos.check_end())throw invalid_iterator();
        node *x = pos.cur;
        if (x->lc != NULL && x->rc != NULL) {
            for (x = x->rc; x->lc != NULL; x = x->lc);
            SWAP(x, pos.cur);
            x = pos.cur;
            /*     delete pos.cur->val;
                 if(x->val!=NULL)pos.cur->val=new value_type(*x->val);
                 else pos.cur->val=NULL,ed=pos.cur;*/
        }
        if (x->lc != NULL || x->rc != NULL) {
            node *y = x->lc == NULL ? x->rc : x->lc;
            y->col = 1;
            y->pre = x->pre;
            if (x == root)root = y;
            else if (x->pre->lc == x)x->pre->lc = y;
            else x->pre->rc = y;
            delete x;
        } else {
            if (x->col)erase_rebalance(x);
            if (x->pre->lc == x)x->pre->lc = NULL;
            else x->pre->rc = NULL;
            delete x;
        }
        --sz;
    }

    /**
     * Returns the number of elements with key
     *   that compares equivalent to the specified argument,
     *   which is either 1 or 0
     *     since this container does not allow duplicates.
     * The default method of check the equivalence is !(a < b || b > a)
     */
    size_t count(const Key &key) const {
        node *x = root;
        while (x != NULL) {
            if (x->val != NULL && !Compare()(key, x->val->first) && !Compare()(x->val->first, key))return 1;
            x = x->val == NULL || Compare()(key, x->val->first) ? x->lc : x->rc;
        }
        return 0;
    }

    /**
     * Finds an element with key equivalent to key.
     * key value of the element to search for.
     * Iterator to an element with key equivalent to key.
     *   If no such element is found, past-the-end (see end()) iterator is returned.
     */
    iterator find(const Key &key) {
        node *x = root;
        while (x != NULL) {
            if (x->val != NULL && !Compare()(key, x->val->first) && !Compare()(x->val->first, key))return iterator(x);
            x = x->val == NULL || Compare()(key, x->val->first) ? x->lc : x->rc;
        }
        return iterator(ed);
    }

    const_iterator find(const Key &key) const {
        node *x = root;
        while (x != NULL) {
            if (x->val != NULL && !Compare()(key, x->val->first) && !Compare()(x->val->first, key))
                return const_iterator(x);
            x = x->val == NULL || Compare()(key, x->val->first) ? x->lc : x->rc;
        }
        return const_iterator(ed);
    }
};

}

#endif
