#include <stdio.h>
#define OLEV    600     /* in feets/minute */
#define MAXALTDIFF 600     /* max altitude difference in feet */
#define MINSEP   300     /* min separation in feet */
#define NOZCROSS  100     /* in feet */
                /* variables */
typedef int bool;
int Cur_Vertical_Sep;
bool High_Confidence;
bool Two_of_Three_Reports_Valid;
int Own_Tracked_Alt;
int Own_Tracked_Alt_Rate;
int Other_Tracked_Alt;
int Alt_Layer_Value;      /* 0, 1, 2, 3 */
int Positive_RA_Alt_Thresh[4];
int Up_Separation;
int Down_Separation;
                /* state variables */
int Other_RAC;         /* NO_INTENT, DO_NOT_CLIMB, DO_NOT_DESCEND */
#define NO_INTENT 0
#define DO_NOT_CLIMB 1
#define DO_NOT_DESCEND 2
int Other_Capability;      /* TCAS_TA, OTHER */
#define TCAS_TA 1
#define OTHER 2
int Climb_Inhibit;       /* true/false */
#define UNRESOLVED 0
#define UPWARD_RA 1
#define DOWNWARD_RA 2
FILE*log;
int probe (char *eid, int eval) {
    fprintf(log, “%s:%d\n”, eid, eval) ;
    return eval ;
}
void initialize()
{
  Positive_RA_Alt_Thresh[0] = 400;
  Positive_RA_Alt_Thresh[1] = 500;
  Positive_RA_Alt_Thresh[2] = 640;
  Positive_RA_Alt_Thresh[3] = 740;
}
int ALIM ()
{
 return Positive_RA_Alt_Thresh[Alt_Layer_Value];
}
int Inhibit_Biased_Climb ()
{
  return (Climb_Inhibit ? Up_Separation + NOZCROSS : Up_Separation);
}
bool Non_Crossing_Biased_Climb()
{
  int upward_preferred;
  int upward_crossing_situation;
  bool result;
  upward_preferred = Inhibit_Biased_Climb() > Down_Separation;
  if (probe(“d1", probe(“c1-1”, upward_preferred)))
  {
    result = !(Own_Below_Threat()) || ((Own_Below_Threat()) && (!(Down_Separation >= ALIM())));
  }
  else
  {
    result = Own_Above_Threat() && (Cur_Vertical_Sep >= MINSEP) && (Up_Separation >= ALIM());
  }
  return result;
}
bool Non_Crossing_Biased_Descend()
{
  int upward_preferred;
  int upward_crossing_situation;
  bool result;
  upward_preferred = Inhibit_Biased_Climb() > Down_Separation;
  if (probe(“d2", probe(“c2-1”, upward_preferred)))
  {
    result = Own_Below_Threat() && (Cur_Vertical_Sep >= MINSEP) && (Down_Separation >= ALIM());
  }
  else
  {
    result = !(Own_Above_Threat()) || ((Own_Above_Threat()) && (Up_Separation >= ALIM()));
  }
  return result;
}
bool Own_Below_Threat()
{
  return (Own_Tracked_Alt < Other_Tracked_Alt);
}
bool Own_Above_Threat()
{
  return (Other_Tracked_Alt < Own_Tracked_Alt);
}
int alt_sep_test()
{
  bool enabled, tcas_equipped, intent_not_known;
  bool need_upward_RA, need_downward_RA;
  int alt_sep;
  enabled = High_Confidence && (Own_Tracked_Alt_Rate <= OLEV) && (Cur_Vertical_Sep > MAXALTDIFF);
  tcas_equipped = Other_Capability == TCAS_TA;
  intent_not_known = Two_of_Three_Reports_Valid && Other_RAC == NO_INTENT;
  alt_sep = UNRESOLVED;
  if (probe(“d3", probe(“c3-1”, enabled) && probe(“c3-2", (probe(“c3-3”, (probe(“c3-4", tcas_equipped) && probe(“c3-5”, intent_not_known)) || probe(“c3-6", !tcas_equipped))))))
  {
    need_upward_RA = Non_Crossing_Biased_Climb() && Own_Below_Threat();
    need_downward_RA = Non_Crossing_Biased_Descend() && Own_Above_Threat();
    if (probe(“d4”, probe(“c4-1", need_upward_RA) && probe(“c4-2”, need_downward_RA)))
      alt_sep = UNRESOLVED; // unreachable
    else if (probe(“d5", probe(“c5-1”, need_upward_RA)))
      alt_sep = UPWARD_RA;
    else if (probe(“d6", probe(“c6-1”, need_downward_RA)))
      alt_sep = DOWNWARD_RA;
    else
      alt_sep = UNRESOLVED;
  }
  return alt_sep;
}
main(argc, argv)
int argc;
char *argv[];
{
  log = fopen(“log.txt”, “w”);
  if(probe(“d0", probe(“c0-1”, argc < 13)))
  {
    fprintf(stdout, “Error: Command line arguments are\n”);
    fprintf(stdout, “Cur_Vertical_Sep, High_Confidence, Two_of_Three_Reports_Valid\n”);
    fprintf(stdout, “Own_Tracked_Alt, Own_Tracked_Alt_Rate, Other_Tracked_Alt\n”);
    fprintf(stdout, “Alt_Layer_Value, Up_Separation, Down_Separation\n”);
    fprintf(stdout, “Other_RAC, Other_Capability, Climb_Inhibit\n”);
    exit(1);
  }
  initialize();
  Cur_Vertical_Sep = atoi(argv[1]);
  High_Confidence = atoi(argv[2]);
  Two_of_Three_Reports_Valid = atoi(argv[3]);
  Own_Tracked_Alt = atoi(argv[4]);
  Own_Tracked_Alt_Rate = atoi(argv[5]);
  Other_Tracked_Alt = atoi(argv[6]);
  Alt_Layer_Value = atoi(argv[7]);
  Up_Separation = atoi(argv[8]);
  Down_Separation = atoi(argv[9]);
  Other_RAC = atoi(argv[10]);
  Other_Capability = atoi(argv[11]);
  Climb_Inhibit = atoi(argv[12]);
fprintf(stdout, “%d\n”, alt_sep_test());
  fclose(log);
  exit(0);
}