# Pseudocode
Height counts edges. An empty tree has height -1; root depth is 0.
All key bounds are strict. Keys are unique lexicographically ordered strings.

## Task 1: Load and build
1. Open the supplied DAT or CSV file; fail clearly if it cannot be opened.
2. For each data line, scan characters while tracking whether inside quotes.
3. Split on commas only outside quotes; decode doubled quotes inside a field.
4. Skip an optional CSV header. Count every airport data row.
5. If IATA is missing (`\N` or an empty CSV field), increment skipped count.
6. Otherwise create Airport(name, city, country, code, ICAO, latitude, longitude).
7. Append the Airport to the records vector in original file order.
8. Start an empty AirportBST; insert each record; report duplicate rejections.
9. Traverse (node, depth) pairs, accumulating count, depth sum and maximum depth.
10. Report height = maximum depth; average depth = depth sum / node count.

### Insert
1. Let link point to the root pointer.
2. While the pointer stored at link is not null:
3.   If new.code is smaller, let link point to the node's left pointer.
4.   Else if larger, let link point to its right pointer.
5.   Else reject the duplicate without changing the tree (I4).
6. Allocate a node at the null link and return success.

## Task 2: Search and extremes
1. Start at root with an empty path and comparisons = 0.
2. While current is not null:
3.   Record its code and increment comparisons.
4.   If the key equals its code, return the airport, path and count.
5.   Move left for a smaller key, otherwise move right.
6. At null, report not found; NULL is not a compared airport.
7. For minimum follow left links; for maximum follow right links.
8. Test root, leaf, deepest node, XYZ, ! (below all keys), ~ (above all keys).

## Task 3: Delete and insertion demonstration
1. Iteratively search for the target, remembering its parent.
2. If absent, return without changing the tree.
3. If the target has two children:
4.   Set successorParent = target; successor = target.right.
5.   Follow left links, updating successorParent, to find the successor.
6.   Copy the successor's entire Airport record into the target.
7.   If successorParent equals target, set target.right = successor.right.
8.   Otherwise set successorParent.left = successor.right.
9.   Free successor (D3 reduced to D1 or D2).
10. Otherwise let child be the only child, or null for a leaf.
11. If target is root, set root = child.
12. Otherwise replace the parent's link to target with child.
13. Free target (D1 or D2).
14. In each test, print top-three-level order before and after; validate.
15. Independently test leaf, left-only, right-only, nonroot two-child and root.
16. Insert QQQ with Somber Xie's test airport record, validate, find, remove,
    validate, and confirm it is absent again.

## Task 4: Traversals, save/load, validation
### Inorder
1. Use an explicit node stack and current = root.
2. Push the entire left path.
3. Pop a node, output its record, then move to its right subtree.
4. Continue until current is null and the stack is empty.
5. Confirm each adjacent pair of codes is strictly increasing.

### Preorder and save
1. Push root onto a stack if nonempty.
2. Repeatedly pop a node and output its code.
3. Push right child, then left child, if present.
4. Save one code per line.

### Load saved preorder
1. Open saved codes; create an empty temporary tree.
2. Sort the loaded master-data catalog by code.
3. For each saved code, binary-search its Airport record in the catalog.
4. Reject unknown or duplicate saved codes, preserving the old tree.
5. Insert each resolved record into the temporary tree in saved order.
6. On success, replace the old tree with the temporary tree.
7. Compare preorder, full level order and height before/after reload.

### Level order
1. Enqueue (root, depth 0) if present.
2. Dequeue a node, output its code, enqueue left then right children.
3. For top-three-level output, do not enqueue children below depth 2.

### Validate using inherited ranges
1. Push (root, no lower bound, no upper bound), if present.
2. Pop (node, lo, hi).
3. Fail if lo exists and node.code <= lo, or hi exists and node.code >= hi.
4. Push right child with (node.code, hi).
5. Push left child with (lo, node.code).
6. Return true when all nodes pass.
7. Run after every insertion/deletion in the mutation tests.

## Task 5: Pruned range query
1. If FROM > TO, return empty with zero visits.
2. Start an explicit stack of (root, examine), if present.
3. Pop an entry; if it is deferred output, append the Airport without recounting.
4. Otherwise increment visited once for this examined node.
5. If code < TO, push the right child for examination, if present.
6. If FROM <= code <= TO, push this node for deferred output.
7. If FROM < code, push the left child for examination, if present.
8. LIFO order processes left subtree, current node, then right subtree.
9. Return the alphabetically ordered records and visit count.

## Task 6: Shape experiment
### Balanced build (middle rule)
1. Obtain the sorted Airport vector from inorder traversal.
2. BUILD(lo, hi): return null when lo > hi.
3. Set mid = lo + (hi - lo) / 2; allocate the middle Airport as node.
4. Set node.left = BUILD(lo, mid - 1).
5. Set node.right = BUILD(mid + 1, hi).
6. Return node. Recursion depth is O(log n), unlike sorted insertion.

### Experiment
1. Read the same 6072 records once.
2. Make sorted and reproducibly shuffled copies (mt19937 seed 2560).
3. Generate 10000 successful random queries with replacement, seed 2560.
4. Build file-order, sorted-order, middle-rule balanced and shuffled trees.
5. Validate each tree and compute its height and average node depth.
6. Time the same queries with steady_clock; exclude build and console output.
7. Search iteratively without collecting paths; accumulate comparisons and
   a checksum so the measured results are consumed.
8. Print and save the measured table. Report timing as environment-dependent.
