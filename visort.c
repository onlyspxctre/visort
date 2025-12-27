#include <raylib.h>

#include <assert.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "sorts.h"

#define WIDTH 800
#define HEIGHT 600

#define N WIDTH

struct Rect {
    float width;
    float height;
};

atomic_int data[N];
int max;
struct Rect rects[N];

void* sort(void* packet) {
    for (int i = 0; i < N; ++i) {
        data[i] = i + 1;
    }

    max = data[find_max(data, N)];

    shuffle(data, N);

    for (size_t i = 0; i < N; ++i) {
        printf("%d\n", data[i]);
        rects[i] = (struct Rect) {
            .width = (float) WIDTH / N,
            .height = ((float) data[i] / (float) max) * HEIGHT,
        };
    }

    visort_quicksort(data, 0, N);

    printf("DONE\n");
    return NULL;
}

int main(void) {
    pthread_t thread;
    pthread_create(&thread, NULL, &sort, NULL);
    pthread_detach(thread);

    InitWindow(WIDTH, HEIGHT, "visort");

    while (!WindowShouldClose()) {
        BeginDrawing();

        if (N > WIDTH) {
            for (size_t i = 0; i < WIDTH; ++i) {
                size_t idx = (size_t) (((float) i / WIDTH) * N);
                const float height = ((float) data[idx] / (float) max) * HEIGHT;
                DrawRectangleRec(
                    (Rectangle) {
                        (float) idx * rects[idx].width,
                        (float) HEIGHT - height,
                        1,
                        height,
                    },
                    (Color) {255, 255, 255, 255});
            }
        } else {
            for (size_t i = 0; i < N; ++i) {
                const float height = ((float) data[i] / (float) max) * HEIGHT;
                DrawRectangleRec(
                    (Rectangle) {
                        (float) i * rects[i].width,
                        (float) HEIGHT - height,
                        rects[i].width,
                        height,
                    },
                    (Color) {255, 255, 255, 255});
            }
        }

        DrawRectangleRec(
            (Rectangle) {
                (float) visort_redbar,
                0,
                1,
                HEIGHT,
            },
            (Color) {255, 0, 0, 255});

        ClearBackground(BLACK);
        EndDrawing();
    }

    CloseWindow();
}
