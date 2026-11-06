#pragma once

// Event.h: the contract between the model and everything that watches it
// (GUI, undo, replay, tests). Field meanings are FROZEN in docs/events.md.
// Changing anything here requires both members to agree.
//
// Event is a plain copyable struct on purpose: EventLog stores it in a
// hand-grown raw buffer, so it must have no destructor, no virtuals, no STL.

enum class EventType
{
    // ---- tree events ----
    VISIT,
    COMPARE,
    INSERT,
    UNLINK,
    SWAP_VALUE,
    ROTATE,
    UPDATE_HEIGHT,
    UPDATE_SIZE,
    BALANCE_ALERT,
    HIGHLIGHT_SUBTREE,

    // ---- heap events ----
    HEAP_BUILD,
    HEAP_COMPARE,
    HEAP_SWAP,
    HEAPIFY_START,
    HEAPIFY_DONE,
    HEAP_INSERT,
    EXTRACT_MAX,
    MARK_SORTED,
    HEAP_SIZE_CHANGE,
    HEAP_SORT_COMPLETE,

    // ---- batch terminator ----
    DONE,

    // Keep LAST. Not a real event; used to loop over all types in tests.
    EVENT_TYPE_COUNT
};

struct Event
{
    EventType type = EventType::DONE;
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    int e = 0;
    const char *note = nullptr; // string literal only, never owned or freed
};

// ---- named constants (use these instead of magic numbers) ----

// Child side (INSERT.c, UNLINK.c, ROTATE.e)
constexpr int SIDE_LEFT = 0;
constexpr int SIDE_RIGHT = 1;
constexpr int SIDE_ROOT = -1;

// ROTATE.b
constexpr int ROT_LL = 0;
constexpr int ROT_RR = 1;
constexpr int ROT_LR = 2;
constexpr int ROT_RL = 3;

// HEAP_SWAP.e
constexpr int REASON_BUILD = 0;
constexpr int REASON_SIFT_UP = 1;
constexpr int REASON_SIFT_DOWN = 2;
constexpr int REASON_EXTRACT = 3;

// HIGHLIGHT_SUBTREE.b
constexpr int HL_VISITED = 0;
constexpr int HL_SKIPPED = 1;
constexpr int HL_RESULT = 2;

// COMPARE.c / HEAP_COMPARE.c
constexpr int CMP_LESS = -1;
constexpr int CMP_EQUAL = 0;
constexpr int CMP_GREATER = 1;

// DONE.c
constexpr int RESULT_FAIL = 0;
constexpr int RESULT_SUCCESS = 1;

// ---- helpers (defined in EventLog.cpp) ----

// Build an event with every unused field zeroed. All emits go through this.
Event makeEvent(EventType type, int a = 0, int b = 0, int c = 0, int d = 0,
                int e = 0, const char *note = nullptr);

// Name of an event type as a string literal ("UNKNOWN" for invalid values).
const char *eventTypeName(EventType type);
