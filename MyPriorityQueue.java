/**
 * Min-heap implementation of a priority queue
 *
 * @author Benjamin Kuperman (Spring 2011)
 * @author Jason Maloy
 */

import java.util.*;

public class MyPriorityQueue<T> extends AbstractQueue<T> {
    
    /** Item to use for making comparisons */
    private Comparator<? super T> cmp;

    /** ArrayList for the min-heap itself */
    private ArrayList<T> heap;

    public void setComparator(Comparator<T> cmp) {
	this.cmp = cmp;
	
	// The new comparator changes how the heap works,
	// so we re-heapify it.
	for (int i = heap.size()/2; i > 0; i--) {
	    percolateDown(i);
	}
    }

    /**
     * @return The comparator used to make this priority queue
     */
    public Comparator<? super T> comparator() {
        return this.cmp;
    }

    public int size() {
        return heap.size();
    }
    
    /**
     * Clears the MyPriorityQueue.
     * This implementation clears by calling clear() on the underlying ArrayList structure
     */
    public void clear() {
        heap.clear();
    }
    
    /**
     * @return The highest priority item in the queue
     */
    public T peek() {
        return heap.get(0);
    }

    /**
     * Remove and return the highest priority item in the queue
     * @return The highest priority item in the queue (which was removed)
     */
    public T poll() {
        T retval = heap.remove(0);
        percolateDown(0);
        return retval;
    }
    
    /**
     * Adds an item to the priority queue
     * @param item Item to be added
     * @return true if the item was correctly added
     */
    public boolean offer(T item) {
        heap.add(0, item);
        percolateDown(0);
        return true;
    }
    
    public Iterator<T> iterator() {
        return new Iterator<T>() {
            int next = 0;
            Boolean canRemove = false;
            
	    @Override
	    public boolean hasNext() {
		return next < heap.size();
	    }

	    @Override
	    public T next() {
		if (!hasNext()) throw new NoSuchElementException("No such element at index " + next);
		canRemove = true;
		return heap.get(next++);	// return the next element, then increment next
	    }

	    /**
	     * Removes the last iterated object.
	     * @throw IllegalStateException If next() was not called since the last call of remove()
	     */
	    @Override
	    public void remove() {
		if (!canRemove) throw new IllegalStateException("Cannot remove without first calling next()");
		heap.remove(next-1);
		canRemove = false;
		
	    }
            
        };
    }
    
    /**
     * Recursively swaps the element at index hole with its lesser child
     * until it either has no children or is greater than its children
     * @param hole Index at which to begin percolation
     */
    private void percolateDown(int hole) {
	/*
	 * Algorithm:
	 * Check which value is the smallest between hole and its children (if they exist).
	 * If smaller != hole, we swap hole and smaller
	 * then recurse on the smallest
	 */
	int smaller = hole;	// index of the smallest between hole and its children
				// only gets changed if a child is smaller
	
	if (leftChild(hole) < 0 && rightChild(hole) < 0) {}	// if the node has no children, we're done
	
	else {
	    if (leftChild(hole) > 0
		    && cmp.compare(heap.get(smaller),
			    heap.get(leftChild(hole))) > 0) { // left child is smaller
		smaller = leftChild(hole);
	    }

	    if (rightChild(hole) > 0
		    && cmp.compare(heap.get(smaller),
			    heap.get(rightChild(hole))) > 0) { // right child is smaller
		smaller = rightChild(hole);
	    }

	    if (smaller != hole) { // If hole wasn't the smallest, we're not done
		swap(hole, smaller);
		percolateDown(smaller);
	    }
	}

    }

    /**
     * Recursively swaps the element at index hole with its greater parent
     * until the element is less than its parent
     * @param hole Index at which to begin percolation
     */
    private void percolateUp(int hole) {
	if (parent(hole) > 0 &&
		cmp.compare( heap.get(hole),
			heap.get(parent(hole))) > 0) {	// If hole is greater than its parent
	    swap(hole, parent(hole));
	    percolateUp(parent(hole));	// Swap and then percolate from the parent index
	}
    }
    
    /**
     * Construct a heap with a given comparator.
     * @param cmp Comparator to use for ordering nodes
     */
    public MyPriorityQueue(Comparator<? super T> cmp) {
        heap = new ArrayList<T>();
        this.cmp = cmp;
    }

    /**
     * Calculate the parent index of a node in a complete binary tree
     * 
     * @param index
     *            node to find parent index of
     * @return index of parent or -1 if there is no parent
     */
    private int parent(int x) {
        if ((x-1)/2 < 0) return -1;
        return (x-1)/2;
    }

    /**
     * Calculate the index for the left child of the given index in a complete
     * binary tree.
     * @param index node to find left child of
     * @return index of left child or -1 if there is no left child
     */
    private int leftChild(int x) {
        if (2*x + 1 > heap.size()-1) return -1;
        return 2*x + 1;
    }

    /**
     * Calculate the index for the right child of the given index in a complete
     * binary tree.
     * @param index node to find right child of
     * @return index of right child or -1 if there is no right child
     */
    private int rightChild(int x) {
        if (2*x + 2 > heap.size()-1) return -1;
        return 2*x + 2;
    }
    
    /**
     * Swaps the data at two indices in the heap
     */
    private void swap(int index1, int index2) {
	T data1 = heap.get(index1);
	T data2 = heap.get(index2);
	heap.set(index1, data2);
	heap.set(index2, data1);
    }
    
    public boolean isEmpty() {
	return heap.isEmpty();
    }

    
}
