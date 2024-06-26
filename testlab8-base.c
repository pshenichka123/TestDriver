#include "testLab.h"
#include "testlab8-base.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <math.h>


enum {

    VERTEX_FOR_LARGES = 5000,
    MAX_VERTEX_COUNT = 20000
};

static unsigned TestcaseIdx = 0;

static int Feed(void);
static int Check(void);




TLabTest GetLabTest(int testIdx) {


    TLabTest labTest[] =
    { {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {Feed, Check},
     {SpecialFeed, Check},
    };
    return labTest[testIdx];


}

static const unsigned TestcaseCount = 35;

int GetTestCount(void) {
    return TestcaseCount;
}





const char* GetTesterName(void) {
    return GetTesterName8_x();
}


int LabTimeout = 3000;
int GetTestTimeout(void) {

    return LabTimeout;

}

size_t LabMemoryLimit = MIN_PROCESS_RSS_BYTES;
size_t GetTestMemoryLimit(void) {

    return LabMemoryLimit;


}



TTestcaseData MakeInteger(unsigned long long integer) {
    TTestcaseData testcaseData;
    testcaseData.Integer = integer;
    return testcaseData;
}

TTestcaseData MakeString(const char* string) {
    TTestcaseData testcaseData;
    testcaseData.String = string;
    return testcaseData;
}

TTestcaseData MakeEdge(unsigned begin, unsigned end, unsigned long long length) {
    TTestcaseData testcaseData;
    testcaseData.Edge.Begin = begin;
    testcaseData.Edge.End = end;
    testcaseData.Edge.Length = length;
    return testcaseData;
}

unsigned SumRange(unsigned begin, unsigned end) {
    return (begin + end) * (end - begin + 1) / 2;
}

void CalcRowColumn(unsigned linearIdx, unsigned* rowIdx, unsigned* columnIdx) {
    *rowIdx = (unsigned)(sqrt(8 * linearIdx + 1) / 2 - 0.5);
    *columnIdx = linearIdx - SumRange(0, *rowIdx);
}







static TTestcaseData GetFromTestcase(unsigned testcaseIdx, enum ETestcaseDataId dataId, unsigned edgeIdx) {
    const unsigned smallTestCount = 30;
    if (testcaseIdx < smallTestCount) {
        typedef struct TSmallTest {
            unsigned VertexCount;
            unsigned EdgeCount;
            struct TEdge Edges[8];
            const char* Message;
            unsigned long long MstLength;
        } TSmallTest;

        static const TSmallTest smallTests[] = {
            {3, 3, {{1, 2, 10}, {2, 3, 5}, {3, 1, 5}}, NULL, 10},
            {3, 1, {{1, 2, 10}}, "no spanning tree"},
            {2, 1, {{IGNORED_VERTEX_IDX}}, "bad number of lines"},

            {0, 0, {{IGNORED_VERTEX_IDX}}, "no spanning tree"},
            {MAX_VERTEX_COUNT + 1, 1, {{1, 1, 1}}, "bad number of vertices"},
            {2, 4, {{1, 1, 1}, {1, 2, 1}, {2, 1, 1}, {2, 2, 1}}, "bad number of edges"},
            {2, 2, {{1, 2, 2}, {1, 2, 1}}, "bad number of edges"},
            {2, 1, {{1, 2, (unsigned long long) - 1}}, "bad length"},

            {2, 0, {{IGNORED_VERTEX_IDX}}, "no spanning tree"},
            {2, 1, {{1, 2, (unsigned long long)4 * INT_MAX}}, "bad length"},
            {4, 2, {{1, 2, INT_MAX}, {2, 3, INT_MAX}}, "no spanning tree"},
            {2, 1, {{1, 1, INT_MAX}}, "no spanning tree"},

            {1, 0, {{IGNORED_VERTEX_IDX}}, NULL, 0},
            {4, 4, {{1, 2, 1}, {2, 3, 2}, {3, 4, 4}, {4, 1, 8}}, NULL, 7},
            {3, 2, {{1, 2, INT_MAX}, {2, 3, INT_MAX}}, NULL, 2ull * INT_MAX},
            {3, 3, {{1, 2, INT_MAX}, {2, 3, INT_MAX}, {1, 3, 1}}, NULL, 1ull + INT_MAX},

            {4, 4, {{1, 2, INT_MAX}, {2, 3, INT_MAX}, {3, 4, INT_MAX}, {4, 1, INT_MAX}}, NULL, 3ull * INT_MAX},
            {4, 4, {{1, 2, 1u << 28}, {2, 3, 2u << 28}, {3, 4, 3u << 28}, {4, 1, 4u << 28}}, NULL, 6ull << 28},
            {4, 4, {{1, 2, (1u << 28) + 4}, {2, 3, (2u << 28) + 3}, {3, 4, (3u << 28) + 2}, {4, 1, (4u << 28) + 1}}, NULL, (6ull << 28) + 9},
            {4, 4, {{1, 2, 1}, {2, 3, 1}, {3, 4, 1}, {4, 1, 1}}, NULL, 3},
            {5, 4, {{1, 2, 1}, {2, 3, 1}, {3, 1, 1}, {4, 3, 1}}, "no spanning tree"},
            {4, 6, {{1, 2, 1}, {1, 3, 2}, {1, 4, 4}, {2, 3, 8}, {2, 4, 16}, {3, 4, 32}}, NULL, 7},

            {3, 2, {{1, 2, 1}, {2, 4, 1}}, "bad vertex"},
            {3, 2, {{1, 2, 1}, {4, 2, 1}}, "bad vertex"},
            {3, 2, {{1, 2, 1}, {(unsigned)-1, 2, 1}}, "bad vertex"},
            {3, 2, {{1, 2, 1}, {2, (unsigned)-1, 1}}, "bad vertex"},

            {4, 4, {{1, 2, 1}, {2, 3, 2}, {1, 3, 3}, {4, 3, 4}}, NULL, 7},
            {4, 3, {{1, 2, 1}, {3, 4, 2}, {2, 4, 3}}, NULL, 6},
            {6, 6, {{1, 2, 1}, {2, 3, 2}, {4, 5, 3}, {5, 6, 4}, {3, 4, 5}, {1, 6, 6}}, NULL, 15},

            {6, 6, {{1, 2, 1}, {3, 4, 2}, {5, 6, 3}, {2, 3, 4}, {4, 5, 5}, {1, 6, 6}}, NULL, 15},
        };
        if (smallTestCount != sizeof(smallTests) / sizeof(smallTests[0])) {
            abort();
        }
        const TSmallTest* test = &smallTests[testcaseIdx];
        switch (dataId) {
        case VERTEX_COUNT:
            return MakeInteger(test->VertexCount);
        case EDGE_COUNT:
            return MakeInteger(test->EdgeCount);
        case EDGE:
            assert(edgeIdx < test->EdgeCount);
            return MakeEdge(test->Edges[edgeIdx].Begin, test->Edges[edgeIdx].End, test->Edges[edgeIdx].Length);
        case ERROR_MESSAGE:
            return MakeString(test->Message);
        case MST_LENGTH:
            return MakeInteger(test->MstLength);
        default:
            abort();
        }
    }
    else if (testcaseIdx == smallTestCount) {
        switch (dataId) {
        case VERTEX_COUNT:
        case EDGE_COUNT:
            return MakeInteger(VERTEX_FOR_LARGES);
        case EDGE:
            assert(edgeIdx < VERTEX_FOR_LARGES);
            return MakeEdge(edgeIdx + 1, (edgeIdx + 1) % VERTEX_FOR_LARGES + 1, edgeIdx + 1);
        case ERROR_MESSAGE:
            return MakeString(NULL);
        case MST_LENGTH:
            return MakeInteger(SumRange(1, VERTEX_FOR_LARGES - 1));
        default:
            abort();
        }
    }
    else if (testcaseIdx == smallTestCount + 1 || testcaseIdx == smallTestCount + 2) {
        const unsigned partOneEdgeCount = VERTEX_FOR_LARGES - 2;
        const unsigned partTwoEdgeCount = VERTEX_FOR_LARGES - 3;
        const int slope = testcaseIdx == 28 ? 1 : -1;
        const int bias = testcaseIdx == 28 ? 0 : VERTEX_FOR_LARGES + 1;
        switch (dataId) {
        case VERTEX_COUNT:
            return MakeInteger(VERTEX_FOR_LARGES);
        case EDGE_COUNT:
            return MakeInteger(partOneEdgeCount + partTwoEdgeCount + 1);
        case EDGE:
            assert(edgeIdx < partOneEdgeCount + partTwoEdgeCount + 1);
            if (edgeIdx < partOneEdgeCount) {
                return MakeEdge((edgeIdx + 1) * slope + bias, (edgeIdx + 2) * slope + bias, edgeIdx + 1);
            }
            else if (edgeIdx < partOneEdgeCount + partTwoEdgeCount) {
                return MakeEdge(1 * slope + bias, (edgeIdx - partOneEdgeCount + 3) * slope + bias, edgeIdx + 3);
            }
            else {
                return MakeEdge((VERTEX_FOR_LARGES - 1) * slope + bias, VERTEX_FOR_LARGES * slope + bias, 2 * VERTEX_FOR_LARGES - 2);
            }
        case ERROR_MESSAGE:
            return MakeString(NULL);
        case MST_LENGTH:
            return MakeInteger(SumRange(1, VERTEX_FOR_LARGES - 2) + 2 * VERTEX_FOR_LARGES - 2);
        default:
            abort();
        }
    }
    else if (testcaseIdx == smallTestCount + 3) {
        const unsigned partOneEdgeCount = VERTEX_FOR_LARGES - 2;
        const unsigned partTwoEdgeCount = SumRange(VERTEX_FOR_LARGES * 4 / 5, VERTEX_FOR_LARGES - 3);
        switch (dataId) {
        case VERTEX_COUNT:
            return MakeInteger(VERTEX_FOR_LARGES);
        case EDGE_COUNT:
            return MakeInteger(partOneEdgeCount + partTwoEdgeCount + 1);
        case EDGE:
            assert(edgeIdx < partOneEdgeCount + partTwoEdgeCount + 1);
            if (edgeIdx < partOneEdgeCount) {
                return MakeEdge(edgeIdx + 1, edgeIdx + 2, edgeIdx + 1);
            }
            else if (edgeIdx < partOneEdgeCount + partTwoEdgeCount) {
                unsigned row, column;
                CalcRowColumn(edgeIdx - partOneEdgeCount + SumRange(0, VERTEX_FOR_LARGES * 4 / 5 - 1), &row, &column); // row = N * 4 / 5 ... N - 3, column = 0 ... row
                row = VERTEX_FOR_LARGES - row;
                return MakeEdge(row - 3, column + row, VERTEX_FOR_LARGES - 1);
            }
            else {
                return MakeEdge(VERTEX_FOR_LARGES - 1, VERTEX_FOR_LARGES, VERTEX_FOR_LARGES);
            }
        case ERROR_MESSAGE:
            return MakeString(NULL);
        case MST_LENGTH:
            return MakeInteger((VERTEX_FOR_LARGES - 1) * (VERTEX_FOR_LARGES - 2) / 2 + VERTEX_FOR_LARGES);
        default:
            abort();
        }
    }
    else if (testcaseIdx == smallTestCount + 4) {
        return Lab8SpecialTest(dataId, edgeIdx);
    }
    else {
        abort();
    }
}

static unsigned GetVertexCount(void) {
    return (unsigned)GetFromTestcase(TestcaseIdx, VERTEX_COUNT, IGNORED_EDGE_IDX).Integer;
}

static unsigned GetEdgeCount(void) {
    return (unsigned)GetFromTestcase(TestcaseIdx, EDGE_COUNT, IGNORED_EDGE_IDX).Integer;
}

static int Feed(void) {
    FILE* const in = fopen("in.txt", "w+");
    if (in == NULL) {
        printf("can't create in.txt. No space on disk?\n");
        return -1;
    }

    const unsigned vertexCount = GetVertexCount();
    const unsigned edgeCount = GetEdgeCount();
    fprintf(in, "%u\n%u\n", vertexCount, edgeCount);
    const int isVerbose = edgeCount > 1000 * 1000;
    if (isVerbose) {
        printf("Creating large text... ");
        fflush(stdout);
    }

    unsigned start = GetTickCount();
    for (unsigned idx = 0; idx < edgeCount; ++idx) {
        const struct TEdge edge = GetFromTestcase(TestcaseIdx, EDGE, idx).Edge;
        if (edge.Begin == IGNORED_VERTEX_IDX) {
            break;
        }
        if (fprintf(in, "%u %u %llu\n", edge.Begin, edge.End, edge.Length) < 3) {
            printf("can't create in.txt. No space on disk?\n");
            fclose(in);
            return -1;
        }
    }
    fclose(in);
    start = RoundUptoThousand(GetTickCount() - start);

    if (isVerbose) {
        printf("done in T=%u seconds. Starting exe with timeout T+3 seconds... ", start / 1000);
        fflush(stdout);
    }

    LabTimeout = (int)start + 3000;
    LabMemoryLimit = vertexCount * vertexCount * 4 + MIN_PROCESS_RSS_BYTES;

    return 0;
}

static unsigned FindEdge(unsigned a, unsigned b) {
    const unsigned vertexCount = GetVertexCount();
    if (a < 1 || a > vertexCount || b < 1 || b > vertexCount) {
        return IGNORED_EDGE_IDX;
    }
    const unsigned edgeCount = GetEdgeCount();
    for (unsigned idx = 0; idx < edgeCount; ++idx) {
        const struct TEdge edge = GetFromTestcase(TestcaseIdx, EDGE, idx).Edge;
        if ((edge.Begin == a && edge.End == b) || (edge.Begin == b && edge.End == a)) {
            return idx;
        }
    }
    return IGNORED_EDGE_IDX;
}

int FindRoot(unsigned vertex, const unsigned parent[]) {
    while (1) {
        if (vertex == parent[vertex]) {
            return vertex;
        }
        vertex = parent[vertex];
    }
}

int CountRoots(unsigned vertexCount, const unsigned parent[]) {
    int rootCount = 0;
    for (unsigned i = 0; i < vertexCount; ++i) {
        if (parent[i] == i) {
            ++rootCount;
        }
    }
    return rootCount;
}

void InitParent(unsigned vertexCount, unsigned parent[]) {
    for (unsigned i = 0; i < vertexCount; ++i) {
        parent[i] = i;
    }
}

static int Check(void) {

    FILE* const out = fopen("out.txt", "r");
    if (out == NULL) {
        printf("can't open out.txt\n");
        ++TestcaseIdx;
        return -1;
    }
    const char* status = Pass;
    const char* message = GetFromTestcase(TestcaseIdx, ERROR_MESSAGE, IGNORED_EDGE_IDX).String;
    if (message != NULL) { // test error message
        char bufMsg[128] = { 0 };
        status = ScanChars(out, sizeof(bufMsg), bufMsg);
        if (status == Pass && _strnicmp(message, bufMsg, strlen(message)) != 0) {
            printf("wrong output -- ");
            status = Fail;
        }
    }
    else { // test spanning tree

        const unsigned vertexCount = GetVertexCount();
        unsigned* vertexParent = (unsigned*)malloc(vertexCount * sizeof(int));
        unsigned long long length = 0;
        InitParent(vertexCount, vertexParent);
        for (unsigned idx = 0; idx + 1 < vertexCount; ++idx) {
            unsigned a, b;

            if (ScanUintUint(out, &a, &b) != Pass) {
                status = Fail;
                break;
            }
            const unsigned edgeIdx = FindEdge(a, b);
            if (edgeIdx == IGNORED_EDGE_IDX) {
                printf("wrong output -- ");
                status = Fail;
                break;
            }
            const unsigned rootA = FindRoot(a - 1, vertexParent);
            const unsigned rootB = FindRoot(b - 1, vertexParent);
            if (rootA == rootB) {
                printf("wrong output -- ");
                status = Fail;
                break;
            }
            vertexParent[rootA] = rootB;
            length += GetFromTestcase(TestcaseIdx, EDGE, edgeIdx).Edge.Length;
        }
        if (status == Pass) {
            if (CountRoots(vertexCount, vertexParent) != 1 || length > GetFromTestcase(TestcaseIdx, MST_LENGTH, IGNORED_EDGE_IDX).Integer) {
                printf("wrong output -- ");
                status = Fail;
            }
        }
    }
    if (status == Pass && HaveGarbageAtTheEnd(out)) {
        status = Fail;
    }
    fclose(out);
    printf("%s\n", status);
    ++TestcaseIdx;
    return status == Fail;
}
