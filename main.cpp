#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

#define ROSTER_SIZE 10

float roster[ROSTER_SIZE];

struct player {
   float score;
   u32 turns;
   void pick(float f) {
      score += f;
      turns += 1;
   }
   void reset() {
      score = 0;
      turns = 0;
   }
};

float frand()
{
   return (float)rand()/RAND_MAX;
}

i32 countbits(i32 in)
{
   i32 res = 0;
   while (in) {
      in = in & (in - 1);
      res++;
   }
   return res;
}

i32 cmpfloat(const void * a, const void * b)
{
   float A = *(float*)a;
   float B = *(float*)b;
   if (A > B) {
      return 1;
   }
   if (A < B) {
      return -1;
   }
   return 0;
}

int main(int argc, char ** argv)
{
   srand(time(0));
   for (i32 i = 0; i < ROSTER_SIZE; i++) {
      roster[i] = frand() * 1000;
   }
   qsort(roster, ROSTER_SIZE, sizeof(float), cmpfloat);
   {
#if 1
      float rosternorm = 0;
      for (i32 i = 0; i < ROSTER_SIZE; i++) {
         rosternorm += (roster[i]);
      }
      rosternorm = 1.f/(rosternorm);
      for (i32 i = 0; i < ROSTER_SIZE; i++) {
         roster[i] *= rosternorm;
      }
#endif
   }
   printf("the roster:\n");
   float roster_total = 0;
   for (i32 i = 0; i < ROSTER_SIZE; i++) {
#if (ROSTER_SIZE <= 10)
      printf("%4.2f ", roster[i]);
#endif
      roster_total += roster[i];
   }
   printf("\n%4.2f points total\n", roster_total);
   {
      //normal turn sequence
      i32 roster_pick;
      player players[2] = {};
      {
         roster_pick = ROSTER_SIZE - 1;
         for (i32 i = 0; i < ROSTER_SIZE; i++) {
            u32 ply = (i % 2);
            player * cur = players + ply;
#if (ROSTER_SIZE <= 10)
            printf("%d picked %f\n", ply, roster[roster_pick]);
#endif
            cur->pick(roster[roster_pick]);
            roster_pick--;
         }
         printf("Normal turn sequence result:\n");
         printf("player 1: %4.3f%% of points, %u turns\n", 100.0 * (players[0].score / roster_total), players[0].turns);
         printf("player 2: %4.3f%% of points, %u turns\n", 100.0 * (players[1].score / roster_total), players[1].turns);
      }
      {
         players[0].reset();
         players[1].reset();
         roster_pick = ROSTER_SIZE - 1;
         for (i32 i = 0; i < ROSTER_SIZE; i++) {
            u32 ply = (countbits(i) % 2);
            player * cur = players + ply;
#if (ROSTER_SIZE <= 10)
            printf("%d picked %f\n", ply, roster[roster_pick]);
#endif
            cur->pick(roster[roster_pick]);
            roster_pick--;
         }
         printf("\"Fair\" turn sequence result:\n");
         printf("player 1: %4.3f%% of points, %u turns\n", 100.0 * (players[0].score / roster_total), players[0].turns);
         printf("player 2: %4.3f%% of points, %u turns\n", 100.0 * (players[1].score / roster_total), players[1].turns);
      }
      {
         players[0].reset();
         players[1].reset();
         roster_pick = ROSTER_SIZE - 1;
         for (i32 i = 0; i < ROSTER_SIZE; i++) {
            u32 ply = (countbits(i + 42) % 2);
            player * cur = players + ply;
#if (ROSTER_SIZE <= 10)
            printf("%d picked %f\n", ply, roster[roster_pick]);
#endif
            cur->pick(roster[roster_pick]);
            roster_pick--;
         }
         printf("magic offset \"Fair\" turn sequence result:\n");
         printf("player 1: %4.3f%% of points, %u turns\n", 100.0 * (players[0].score / roster_total), players[0].turns);
         printf("player 2: %4.3f%% of points, %u turns\n", 100.0 * (players[1].score / roster_total), players[1].turns);
      }
      {
         players[0].reset();
         players[1].reset();
         roster_pick = ROSTER_SIZE - 1;
         i32 randadd;
         for (i32 i = 0; i < ROSTER_SIZE; i++) {
            if (i%2 == 0) {
               randadd = rand()%2;
            }
            u32 ply = ((i + randadd) % 2);
            player * cur = players + ply;
#if (ROSTER_SIZE <= 10)
            printf("%d picked %f\n", ply, roster[roster_pick]);
#endif
            cur->pick(roster[roster_pick]);
            roster_pick--;
         }
         printf("random pair turn sequence result:\n");
         printf("player 1: %4.3f%% of points, %u turns\n", 100.0 * (players[0].score / roster_total), players[0].turns);
         printf("player 2: %4.3f%% of points, %u turns\n", 100.0 * (players[1].score / roster_total), players[1].turns);
      }
   }
}
