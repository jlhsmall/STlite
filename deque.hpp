#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>
#include <mcheck.h>

namespace sjtu {
const size_t S=500;
template<class T>
class deque {
public:
    struct node {
        T *val;
        node *pre, *nxt;

        node() {
            val = NULL;
            pre = nxt = NULL;
        }

        node(T _val, node *_pre = NULL, node *_nxt = NULL) : pre(_pre), nxt(_nxt) {
            val = new T(_val);
        }

        ~node() {
            if (val != NULL)delete val;
        }

        bool check_head() const {
            return pre == NULL;
        }

        bool check_tail() const {
            return nxt == NULL;
        }
    };

    struct NODE {
        node *head, *tail;
        size_t sz;
        NODE *PRE, *NXT;

        NODE(NODE *_PRE = NULL, NODE *_NXT = NULL, size_t _sz = 0) : PRE(_PRE), NXT(_NXT), sz(_sz) {
            head = new node;
            tail = new node;
            head->nxt = tail;
            tail->pre = head;
        }

        NODE(const NODE &other) {
            sz = other.sz;
            node *p = head = new node, *q = other.head->nxt;
            for (; q != other.tail; p = p->nxt, q = q->nxt) {
                p->nxt = new node(*q->val, p);
            }
            p->nxt = tail = new node;
            tail->pre = p;
            PRE = NXT = NULL;
        }

        ~NODE() {
            node *p = head, *q;
            for (; p != tail; p = q) {
                q = p->nxt;
                delete p;
            }
            delete p;
        }

        bool check_HEAD() const {
            return PRE == NULL;
        }

        bool check_TAIL() const {
            return NXT == NULL;
        }

        bool PRE_size_inadequate() const {
            return !PRE->check_HEAD() && PRE->sz + sz <= S;
        }

        bool NXT_size_inadequate() const {
            return !check_TAIL() && !NXT->check_TAIL() && NXT->sz + sz <= S;
        }

        void split(node *pp) {
            node *tail1 = new node, *head2 = new node;
            tail1->pre = pp->pre;
            head2->nxt = pp;
            pp->pre->nxt = tail1;
            pp->pre = head2;
            size_t sz1 = 0, sz2 = 0;
            node *head1, *tail2;
            for (head1 = tail1->pre; !head1->check_head(); head1 = head1->pre)++sz1;
            for (tail2 = head2->nxt; !tail2->check_tail(); tail2 = tail2->nxt)++sz2;
            NODE *preNODE = new NODE(PRE, this, sz1);
            delete preNODE->head;delete preNODE->tail;
            preNODE->head = head;
            preNODE->tail = tail1;
            head = head2;
            sz = sz2;
            PRE->NXT = preNODE;
            PRE = preNODE;
        }

        void merge() {
            tail->pre->nxt = NXT->head->nxt;
            NXT->head->nxt->pre = tail->pre;
            NXT->head->nxt = tail;
            tail->pre = NXT->head;
            std::swap(tail, NXT->tail);
            sz += NXT->sz;
            NODE *p = NXT;
            NXT->NXT->PRE = this;
            NXT = NXT->NXT;
            delete p;
        }
    } *HEAD, *TAIL;

    size_t SZ;

    class const_iterator;

    class iterator {
        //    friend const_iterator;
    private:
    public:
        NODE *CUR;
        node *cur;

        iterator(NODE *_CUR = NULL, node *_cur = NULL) : CUR(_CUR), cur(_cur) {}

        /**
         * return a new iterator which pointer n-next elements
         *   even if there are not enough elements, the behaviour is **undefined**.
         * as well as operator-
         */
        bool check_invalid() const {
            return cur == NULL;
        }

        iterator operator+(const int &n) const {
            if (check_invalid())throw invalid_iterator();
            if (!n)return *this;
            if (n < 0)return *this - (-n);
            iterator ret = *this;
            int k = n;
            while (k && !ret.cur->check_tail())--k, ret.cur = ret.cur->nxt;
            if (!ret.cur->check_tail())return ret;
            while (!ret.CUR->check_TAIL()) {
                ret.CUR = ret.CUR->NXT;
                if (ret.CUR->check_TAIL() || ret.CUR->sz > k)break;
                k -= ret.CUR->sz;
            }
            if (ret.CUR->check_TAIL() && k)ret.cur = NULL;
            else for (ret.cur = ret.CUR->head; k >= 0; --k)ret.cur = ret.cur->nxt;
            return ret;
        }

        iterator operator-(const int &n) const {
            if (check_invalid())throw invalid_iterator();
            if (!n)return *this;
            if (n < 0)return *this + (-n);
            iterator ret = *this;
            int k = n;
            while (k && !ret.cur->check_head())--k, ret.cur = ret.cur->pre;
            if (!ret.cur->check_head())return ret;
            while (1) {
                ret.CUR = ret.CUR->PRE;
                if (ret.CUR->check_HEAD() || ret.CUR->sz > k)break;
                k -= ret.CUR->sz;
            }
            if (ret.CUR->check_HEAD())ret.cur = NULL;
            else for (ret.cur = ret.CUR->tail; k >= 0; --k)ret.cur = ret.cur->pre;
            return ret;
        }

        // return th distance between two iterator,
        // if these two iterators points to different vectors, throw invaild_iterator.
        int operator-(const iterator &rhs) const {
            if (check_invalid() || rhs.check_invalid())throw invalid_iterator();
            iterator p = *this, q = rhs;
            int cntp = 0, cntq = 0;
            while (!p.cur->check_tail())p.cur = p.cur->nxt, ++cntp;
            while (!p.CUR->check_TAIL()) {
                p.CUR = p.CUR->NXT;
                cntp += p.CUR->sz;
            }
            while (!q.cur->check_tail())q.cur = q.cur->nxt, ++cntq;
            while (!q.CUR->check_TAIL()) {
                q.CUR = q.CUR->NXT;
                cntq += q.CUR->sz;
            }
            if (p.CUR != q.CUR)throw invalid_iterator();
            return cntq - cntp;
        }

        iterator &operator+=(const int &n) {
            if (check_invalid())throw invalid_iterator();
            if (!n)return *this;
            if (n < 0) return *this -= (-n);
            int k = n;
            while (k && !cur->check_tail())--k, cur = cur->nxt;
            if (!cur->check_tail())return *this;
            while (!CUR->check_TAIL()) {
                CUR = CUR->NXT;
                if (CUR->check_TAIL() || CUR->sz > k)break;
                k -= CUR->sz;
            }
            if (CUR->check_TAIL() && k)cur = NULL;
            else for (cur = CUR->head; k >= 0; --k)cur = cur->nxt;
            return *this;
        }

        iterator &operator-=(const int &n) {
            if (check_invalid())throw invalid_iterator();
            if (n < 0)return *this += -n;
            if (!n)return *this;
            int k = n;
            while (k && !cur->check_head())--k, cur = cur->pre;
            if (!cur->check_head())return *this;
            while (1) {
                CUR = CUR->PRE;
                if (CUR->check_HEAD() || CUR->sz > k)break;
                k -= CUR->sz;
            }
            if (CUR->check_HEAD())cur = NULL;
            else for (cur = CUR->tail; k >= 0; --k)cur = cur->pre;
            return *this;
        }

        /**
         * TODO iter++
         */
        iterator operator++(int) {
            if (check_invalid())throw invalid_iterator();
            iterator ret = *this;
            *this += 1;
            return ret;
        }

        /**
         * TODO ++iter
         */
        iterator &operator++() {
            if (check_invalid())throw invalid_iterator();
            *this += 1;
            return *this;
        }

        /**
         * TODO iter--
         */
        iterator operator--(int) {
            if (check_invalid())throw invalid_iterator();
            iterator ret = *this;
            *this -= 1;
            return ret;
        }

        /**
         * TODO --iter
         */
        iterator &operator--() {
            if (check_invalid())throw invalid_iterator();
            *this -= 1;
            return *this;
        }

        /**
         * TODO *it
         */
        T &operator*() const {
            if (check_invalid() || CUR->check_TAIL())throw invalid_iterator();
            return *cur->val;
        }

        /**
         * TODO it->field
         */
        T *operator->() const {
            if (check_invalid() || CUR->check_TAIL())throw invalid_iterator();
            return cur->val;
        }

        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return CUR == rhs.CUR && cur == rhs.cur;
        }

        bool operator==(const const_iterator &rhs) const {
            return CUR == rhs.CUR && cur == rhs.cur;
        }

        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return CUR != rhs.CUR || cur != rhs.cur;
        }

        bool operator!=(const const_iterator &rhs) const {
            return CUR != rhs.CUR || cur != rhs.cur;
        }
    };

    class const_iterator {
        // it should has similar member method as iterator.
        //  and it should be able to construct from an iterator.
    private:// data members.
    public:
        NODE *CUR;
        node *cur;

        const_iterator(NODE *_CUR = NULL, node *_cur = NULL) : CUR(_CUR), cur(_cur) {}

        const_iterator(const const_iterator &other) : CUR(other.CUR), cur(other.cur) {}

        const_iterator(const iterator &other) : CUR(other.CUR), cur(other.cur) {}

        bool check_invalid() const {
            return cur == NULL;
        }

        const_iterator operator+(const int &n) const {
            if (check_invalid())throw invalid_iterator();
            if (!n)return *this;
            if (n < 0)return *this - (-n);
            const_iterator ret = *this;
            int k = n;
            while (k && !ret.cur->check_tail())--k, ret.cur = ret.cur->nxt;
            if (!ret.cur->check_tail())return ret;
            while (!ret.CUR->check_TAIL()) {
                ret.CUR = ret.CUR->NXT;
                if (ret.CUR->check_TAIL() || ret.CUR->sz > k)break;
                k -= ret.CUR->sz;
            }
            if (ret.CUR->check_TAIL() && k)ret.cur = NULL;
            else for (ret.cur = ret.CUR->head; k >= 0; --k)ret.cur = ret.cur->nxt;
            return ret;
        }

        const_iterator operator-(const int &n) const {
            if (check_invalid())throw invalid_iterator();
            if (!n)return *this;
            if (n < 0)return *this + (-n);
            const_iterator ret = *this;
            int k = n;
            while (k && !ret.cur->check_head())--k, ret.cur = ret.cur->pre;
            if (!ret.cur->check_head())return ret;
            while (1) {
                ret.CUR = ret.CUR->PRE;
                if (ret.CUR->check_HEAD() || ret.CUR->sz > k)break;
                k -= ret.CUR->sz;
            }
            if (ret.CUR->check_HEAD())ret.cur = NULL;
            else for (ret.cur = ret.CUR->tail; k >= 0; --k)ret.cur = ret.cur->pre;
            return ret;
        }

        // return th distance between two const_iterator,
        // if these two const_iterators points to different vectors, throw invaild_iterator.
        int operator-(const const_iterator &rhs) const {
            if (check_invalid() || rhs.check_invalid())throw invalid_iterator();
            const_iterator p = *this, q = rhs;
            int cntp = 0, cntq = 0;
            while (!p.cur->check_tail())p.cur = p.cur->nxt, ++cntp;
            while (!p.CUR->check_TAIL()) {
                p.CUR = p.CUR->NXT;
                cntp += p.CUR->sz;
            }
            while (!q.cur->check_tail())q.cur = q.cur->nxt, ++cntq;
            while (!q.CUR->check_TAIL()) {
                q.CUR = q.CUR->NXT;
                cntq += q.CUR->sz;
            }
            if (p.CUR != q.CUR)throw invalid_iterator();
            return cntq - cntp;
        }

        const_iterator &operator+=(const int &n) {
            if (check_invalid())throw invalid_iterator();
            if (!n)return *this;
            if (n < 0) return *this -= (-n);
            int k = n;
            while (k && !cur->check_tail())--k, cur = cur->nxt;
            if (!cur->check_tail())return *this;
            while (!CUR->check_TAIL()) {
                CUR = CUR->NXT;
                if (CUR->check_TAIL() || CUR->sz > k)break;
                k -= CUR->sz;
            }
            if (CUR->check_TAIL() && k)cur = NULL;
            else for (cur = CUR->head; k >= 0; --k)cur = cur->nxt;
            return *this;
        }

        const_iterator &operator-=(const int &n) {
            if (check_invalid())throw invalid_iterator();
            if (n < 0)return *this += -n;
            if (!n)return *this;
            int k = n;
            while (k && !cur->check_head())--k, cur = cur->pre;
            if (!cur->check_head())return *this;
            while (1) {
                CUR = CUR->PRE;
                if (CUR->check_HEAD() || CUR->sz > k)break;
                k -= CUR->sz;
            }
            if (CUR->check_HEAD())cur = NULL;
            else for (cur = CUR->tail; k >= 0; --k)cur = cur->pre;
            return *this;
        }

        /**
         * TODO iter++
         */
        const_iterator operator++(int) {
            if (check_invalid())throw invalid_iterator();
            const_iterator ret = *this;
            *this += 1;
            return ret;
        }

        /**
         * TODO ++iter
         */
        const_iterator &operator++() {
            if (check_invalid())throw invalid_iterator();
            *this += 1;
            return *this;
        }

        /**
         * TODO iter--
         */
        const_iterator operator--(int) {
            if (check_invalid())throw invalid_iterator();
            const_iterator ret = *this;
            *this -= 1;
            return ret;
        }

        /**
         * TODO --iter
         */
        const_iterator &operator--() {
            if (check_invalid())throw invalid_iterator();
            *this -= 1;
            return *this;
        }

        /**
         * TODO *it
         */
        const T &operator*() const {
            if (check_invalid() || CUR->check_TAIL())throw invalid_iterator();
            return *cur->val;
        }

        /**
         * TODO it->field
         */
        const T *operator->() const {
            if (check_invalid() || CUR->check_TAIL())throw invalid_iterator();
            return cur->val;
        }

        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const const_iterator &rhs) const {
            return CUR == rhs.CUR && cur == rhs.cur;
        }

        bool operator==(const iterator &rhs) const {
            return CUR == rhs.CUR && cur == rhs.cur;
        }

        /**
         * some other operator for iterator.
         */
        bool operator!=(const const_iterator &rhs) const {
            return CUR != rhs.CUR || cur != rhs.cur;
        }

        bool operator!=(const iterator &rhs) const {
            return CUR != rhs.CUR || cur != rhs.cur;
        }
    };

    /**
     * TODO Constructors
     */
    deque() {
        HEAD = new NODE;
        TAIL = new NODE;
        SZ = 0;
        HEAD->NXT = TAIL;
        TAIL->PRE = HEAD;
    }

    deque(const deque &other) {
        NODE *p = HEAD = new NODE, *q = other.HEAD;
        SZ = other.SZ;
        for (; q != other.TAIL; p = p->NXT, q = q->NXT) {
            p->NXT = new NODE(*(q->NXT));
            p->NXT->PRE = p;
        }
        TAIL = p;
    }

    /**
     * TODO Deconstructor
     */
    ~deque() {
        NODE *p = HEAD, *q;
        for (; p != TAIL; p = q) {
            q = p->NXT;
            delete p;
        }
        delete p;
    }

    /**
     * TODO assignment operator
     */
    deque &operator=(const deque &other) {
        if (this == &other)return *this;
        NODE *p = HEAD, *q;
        //del
        for (; p != TAIL; p = q) {
            q = p->NXT;
            delete p;
        }
        delete p;
        //copy
        p = HEAD = new NODE;
        for (q = other.HEAD; q != other.TAIL; p = p->NXT, q = q->NXT) {
            p->NXT = new NODE(*(q->NXT));
            p->NXT->PRE = p;
        }
        TAIL = p;
        SZ = other.SZ;
        return *this;
    }

    /**
     * access specified element with bounds checking
     * throw index_out_of_bound if out of bound.
     */
    T &at(const size_t &pos) {
        if (pos < 0 || pos >= SZ) throw index_out_of_bound();
        NODE *p = HEAD->NXT;
        size_t k = pos;
        while (!p->check_TAIL() && p->sz <= k) {
            k -= p->sz;
            p = p->NXT;
        }
        node *pp = p->head->nxt;
        while (k--)pp = pp->nxt;
        return *pp->val;
    }

    const T &at(const size_t &pos) const {
        if (pos < 0 || pos >= SZ) throw index_out_of_bound();
        NODE *p = HEAD->NXT;
        size_t k = pos;
        while (!p->check_TAIL() && p->sz <= k) {
            k -= p->sz;
            p = p->NXT;
        }
        node *pp = p->head->nxt;
        while (k--)pp = pp->nxt;
        return *pp->val;
    }

    T &operator[](const size_t &pos) {
        if (pos < 0 || pos >= SZ) throw index_out_of_bound();
        NODE *p = HEAD->NXT;
        size_t k = pos;
        while (!p->check_TAIL() && p->sz <= k) {
            k -= p->sz;
            p = p->NXT;
        }
        node *pp = p->head->nxt;
        while (k--)pp = pp->nxt;
        return *pp->val;
    }

    const T &operator[](const size_t &pos) const {
        if (pos < 0 || pos >= SZ) throw index_out_of_bound();
        NODE *p = HEAD->NXT;
        size_t k = pos;
        while (!p->check_TAIL() && p->sz <= k) {
            k -= p->sz;
            p = p->NXT;
        }
        node *pp = p->head->nxt;
        while (k--)pp = pp->nxt;
        return *pp->val;
    }

    /**
     * access the first element
     * throw container_is_empty when the container is empty.
     */
    const T &front() const {
        if (empty())throw container_is_empty();
        return *HEAD->NXT->head->nxt->val;
    }

    /**
     * access the last element
     * throw container_is_empty when the container is empty.
     */
    const T &back() const {
        if (empty())throw container_is_empty();
        return *TAIL->PRE->tail->pre->val;
    }

    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(HEAD->NXT, HEAD->NXT->head->nxt);
    }

    const_iterator cbegin() const {
        return const_iterator(HEAD->NXT, HEAD->NXT->head->nxt);
    }

    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(TAIL, TAIL->tail);
    }

    const_iterator cend() const {
        return const_iterator(TAIL, TAIL->tail);
    }

    /**
     * checks whether the container is empty.
     */
    bool empty() const { return HEAD->NXT == TAIL; }

    /**
     * returns the number of elements
     */
    size_t size() const { return SZ; }

    /**
     * clears the contents
     */
    void clear() {
        NODE *p = HEAD->NXT, *q;
        for (; p != TAIL; p = q) {
            q = p->NXT;
            delete p;
        }
        HEAD->NXT = TAIL;
        TAIL->PRE = HEAD;
        SZ = 0;
    }

    /**
     * inserts elements at the specified locat on in the container.
     * inserts value before pos
     * returns an iterator pointing to the inserted value
     *     throw if the iterator is invalid or it point to a wrong place.
     */

    iterator Insert(iterator pos, const T &value) {
        ++SZ;
        node *pp = new node(value, pos.cur->pre, pos.cur);
        pos.cur->pre->nxt = pp;
        pos.cur->pre = pp;
        if (++pos.CUR->sz <= S && !pos.cur->check_tail()) {
            return iterator(pos.CUR, pp);
        }
        pos.CUR->split(pos.cur);
        if (pos.CUR->PRE->PRE_size_inadequate()) {
            pos.CUR->PRE->PRE->merge();
        } else if (pos.CUR->NXT_size_inadequate())pos.CUR->merge();
        return pos - 1;
    }

    iterator insert(iterator pos, const T &value) {
        if (pos.check_invalid())throw invalid_iterator();
        NODE *p = pos.CUR;
        while (!p->check_TAIL())p = p->NXT;
        if (p != TAIL)throw invalid_iterator();
        return Insert(pos, value);
    }

    /**
     * removes specified element at pos.
     * removes the element at pos.
     * returns an iterator pointing to the followig element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid or it points to a wrong place.
     */
    iterator Erase(iterator pos) {
        --SZ;
        iterator ret = pos + 1;
        pos.cur->nxt->pre = pos.cur->pre;
        pos.cur->pre->nxt = pos.cur->nxt;
        delete pos.cur;
        --pos.CUR->sz;
        if (pos.CUR->PRE_size_inadequate()) {
            if (pos.CUR == ret.CUR)ret.CUR = ret.CUR->PRE;
            pos.CUR->PRE->merge();
        } else if (pos.CUR->NXT_size_inadequate()) {
            ret.CUR = pos.CUR;
            pos.CUR->merge();
        }
        if (!SZ)clear();
        return ret;
    }

    iterator erase(iterator pos) {
        if (pos.check_invalid() || pos.CUR->check_TAIL())throw invalid_iterator();
        NODE *p = pos.CUR;
        while (!p->check_TAIL())p = p->NXT;
        if (p != TAIL)throw invalid_iterator();
        return Erase(pos);
    }

    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        Insert(end(), value);
    }

    /**
     * removes the last element
     *     throw when the container is empty.
     */
    void pop_back() {
        if (empty())throw invalid_iterator();
        Erase(end() - 1);
    }

    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        Insert(begin(), value);
    }

    /**
     * removes the first element.
     *     throw when the container is empty.
     */
    void pop_front() {
        if (empty())throw invalid_iterator();
        Erase(begin());
    }
};

}

#endif
