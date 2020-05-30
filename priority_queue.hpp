#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
template<typename T, class Compare = std::less<T>>
class priority_queue {
public:
	struct node {
        T val;
        int dist;
        struct node *lc, *rc;

        node(const T &x,const int y=0) : val(x), dist(y), lc(NULL), rc(NULL) {}
    };
	node *TOP;
	size_t SIZE;
	priority_queue() {
	    TOP=NULL;SIZE=0;
	}
	node* cpy(node *y) {
        if (y == NULL) {
            return NULL;
        }
        node *ret = new node(y->val, y->dist);
        ret->lc = cpy(y->lc);
        ret->rc = cpy(y->rc);
        return ret;
    }
	priority_queue(const priority_queue &other) {
	    TOP=cpy(other.TOP);
	    SIZE=other.SIZE;
	}
	/**
	 * TODO deconstructor
	 */
	void del(node *x){
	    if(x==NULL)return;
	    del(x->lc);del(x->rc);
	    delete x;
	}
	~priority_queue() {
	    del(TOP);
	}
	/**
	 * TODO Assignment operator
	 */
	 node *Merge(node *x,node *y) {
        if (y == NULL)return x;
        if (x == NULL)return y;
        if (Compare()(x->val, y->val))std::swap(x, y);
        x->rc = Merge(x->rc, y);
        if (x->lc == NULL || (x->rc != NULL && x->rc->dist > x->lc->dist))std::swap(x->lc, x->rc);
        x->dist = x->rc == NULL ? 0 : x->rc->dist + 1;
        return x;
    }
	priority_queue &operator=(const priority_queue &other) {
	     if(this==&other)return *this;
	     del(TOP);
	     TOP=cpy(other.TOP);
	     SIZE=other.SIZE;
	     return *this;
	 }
	/**
	 * get the top of the queue.
	 * @return a reference of the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	const T & top() const {
	    if(empty())throw container_is_empty();
	    return TOP->val;
	}
	/**
	 * TODO
	 * push new element to the priority queue.
	 */
	void push(const T &e) {
	    node *tmp=new node(e);
	    TOP=Merge(TOP,tmp);
	    ++SIZE;
	}
	/**
	 * TODO
	 * delete the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	void pop() {
        if(empty())throw container_is_empty();
	    node *tmp=TOP;
	    TOP=Merge(TOP->lc,TOP->rc);
	    delete tmp;
	    --SIZE;
	}
	/**
	 * return the number of the elements.
	 */
	size_t size() const {
        return SIZE;
	}
	/**
	 * check if the container has at least an element.
	 * @return true if it is empty, false if it has at least an element.
	 */
	bool empty() const {
        return !SIZE;
    }
	/**
	 * return a merged priority_queue with at least O(logn) complexity.
	 */
	void merge(priority_queue &other) {
        TOP = Merge(TOP, other.TOP);
        SIZE += other.SIZE;
        other.TOP = NULL;
        other.SIZE = 0;
    }
};

}

#endif
