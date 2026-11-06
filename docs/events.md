# Vault Heist: Event Specification (FROZEN)

Changing anything below requires both members to agree and every dependent file to be updated.

## 1. Struct

```cpp
struct Event {
    EventType   type;
    int         a, b, c, d, e;   // meaning depends on type, unused fields = 0
    const char* note;            // optional string literal for the log panel, may be nullptr
};
```

- `0` in a node-id field means "no node". Unused fields are set to `0`.
- Sides: `0 = left`, `1 = right`, `-1 = root`.
- `note` points to a string literal. It is never freed and never owned.
- Undo uses **snapshots first** (serialise before each action). The "undo data" columns below exist so
  inverse-event undo can be added later without changing this table.

## 2. Tree events

| Type | a | b | c | d | e | Visual effect | Undo data |
| --- | --- | --- | --- | --- | --- | --- | --- |
| `VISIT` | nodeId | | | | | Node pulses, alarm +1 | none |
| `COMPARE` | nodeId | target key | result: -1 target < node key (go left), 0 equal, +1 target > node key (go right) | | | Arrow shows direction, alarm +1 | none |
| `INSERT` | nodeId | parentId (0 if root) | side | key | | Node grows in at its slot | unlink nodeId |
| `UNLINK` | nodeId | parentId (0 if root) | side | value | promotedChildId (0 if leaf) | Node fades, edge retracts. For one-child delete, `e` is the child that replaces it | relink nodeId under b/c, restore e |
| `SWAP_VALUE` | nodeA | nodeB | valueA before | valueB before | | Values slide between nodes (delete, two-child case) | swap back using c, d |
| `ROTATE` | pivotId (the unbalanced node) | type: 0 LL, 1 RR, 2 LR, 3 RL | newSubtreeRootId | oldParentId of pivot (0 if root) | side of pivot under old parent | Nodes tween to new positions | reverse links using a, c, d, e |
| `UPDATE_HEIGHT` | nodeId | old | new | | | Badge flickers | restore old |
| `UPDATE_SIZE` | nodeId | old | new | | | Size label flickers | restore old |
| `BALANCE_ALERT` | nodeId | balanceFactor | | | | Badge turns red | none |
| `HIGHLIGHT_SUBTREE` | nodeId | mode: 0 visited, 1 skipped, 2 result | | | | Subtree tinted | none |

Notes:

- A double rotation (LR or RL) emits **one** `ROTATE` with type 2 or 3. The two primitive rotations are
  animated by the GUI as two phases, and the final `newSubtreeRootId` is the grandchild that ends up on top.
- Height and size changes caused by a rotation are emitted as separate `UPDATE_HEIGHT` / `UPDATE_SIZE`
  events **after** the `ROTATE`.
- `COMPARE` is emitted **in addition to** `VISIT` for the node being compared. Alarm cost: entering a room
  (VISIT) and comparing are both specified as 1 step in the spec, so the **level logic**, not the model,
  decides which events count toward the alarm. The model just reports truthfully.

## 3. Heap events

| Type | a | b | c | d | e | Visual effect |
| --- | --- | --- | --- | --- | --- | --- |
| `HEAP_BUILD` | first index to sift (n/2 - 1) | heapSize (= n) | | | | Highlights active region before construction |
| `HEAP_COMPARE` *(addition)* | indexA | indexB | result: -1 A<B, 0 equal, +1 A>B | | | Cells and nodes highlighted during a comparison |
| `HEAP_SWAP` | indexA | indexB | valueA before | valueB before | reason: 0 build, 1 siftUp, 2 siftDown, 3 extract | Both array cells and both tree nodes swap |
| `HEAPIFY_START` | start index | heapSize | | | | Active heap region highlighted |
| `HEAPIFY_DONE` | index where the element settled | heapSize | | | | Restoration indicated |
| `HEAP_INSERT` *(addition)* | index of the new element | value | | | | New cell appears at the end of the active region |
| `EXTRACT_MAX` | value | rootIndex (always 0) | heapSize before extraction | | | Root highlighted as the maximum being extracted |
| `MARK_SORTED` | index | value | | | | Cell moves into the visually distinct sorted region |
| `HEAP_SIZE_CHANGE` | oldSize | newSize | | | | Heap-size indicator updates |
| `HEAP_SORT_COMPLETE` | n (array length) | | | | | Celebration, vault unlocks. Final array is read from the heap itself. |

Two events are additions to the spec table (4.3), justified by other parts of the spec:

- `HEAP_COMPARE`: section 6.2 asks the Event Log to show comparisons, and section 2.4 penalises incorrect parent-child comparisons.
- `HEAP_INSERT`: `insert(value)` must tell the GUI where the new value appears before any `siftUp` swaps.

Heap order of events for common operations:

- `buildMaxHeap`: `HEAP_BUILD`, then for each i from n/2-1 down to 0: `HEAPIFY_START`, (`HEAP_COMPARE`, `HEAP_SWAP`)*, `HEAPIFY_DONE`, then `DONE`.
- `extractMax`: `EXTRACT_MAX`, `HEAP_SWAP` (reason 3), `MARK_SORTED`, `HEAP_SIZE_CHANGE`, then `HEAPIFY_START`, ..., `HEAPIFY_DONE`, then `DONE`.
- `heapSort`: build events, then repeated extract events, then `HEAP_SORT_COMPLETE`, `DONE`.

## 4. Batch terminator

| Type | a | b | c | d | e |
|---|---|---|---|---|---|
| `DONE` | comparisons made | swaps or rotations made | operation result (1 success, 0 failure e.g. duplicate or not found) | | |

Every operation that records events ends with exactly one `DONE`. When `log == nullptr`, nothing is emitted.

## 5. EventType enum (for Event.h)

```cpp
enum class EventType {
    VISIT, COMPARE, INSERT, UNLINK, SWAP_VALUE, ROTATE,
    UPDATE_HEIGHT, UPDATE_SIZE, BALANCE_ALERT, HIGHLIGHT_SUBTREE,
    HEAP_BUILD, HEAP_COMPARE, HEAP_SWAP, HEAPIFY_START, HEAPIFY_DONE,
    HEAP_INSERT, EXTRACT_MAX, MARK_SORTED, HEAP_SIZE_CHANGE, HEAP_SORT_COMPLETE,
    DONE
};
```

Add a small `const char* eventTypeName(EventType)` in `EventLog.cpp` for the log panel and for test output.
Add a `makeEvent(type, a, b, c, d, e, note)` helper to avoid forgetting to zero fields.

## 6. EventLog interface (for EventLog.h)

`emit(const Event&)`, `size() const`, `get(int i) const`, `clear()`, `setEnabled(bool)`, `isEnabled() const`.
Backed by an internal growable `Event` buffer (no STL). `emit` does nothing when disabled.
