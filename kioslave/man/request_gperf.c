/* C++ code produced by gperf version 3.1 */
/* Command-line: gperf -L C++ -F ', REQ_UNKNOWN' -I --null-strings -compare-strncmp requests.gperf  */
/* Computed positions: -k'1,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 2 "requests.gperf"
struct Requests { const char *name; RequestNum number; };
#include <string.h>

#define TOTAL_KEYWORDS 156
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 5
#define MIN_HASH_VALUE 6
#define MAX_HASH_VALUE 467
/* maximum key range = 462, duplicates = 0 */

class Perfect_Hash
{
private:
  static inline unsigned int hash (const char *str, size_t len);
public:
  static struct Requests *in_word_set (const char *str, size_t len);
};

inline unsigned int
Perfect_Hash::hash (const char *str, size_t len)
{
  static unsigned short asso_values[] =
    {
      468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
      468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
      468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
      468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
      185, 468, 468, 468, 468, 468, 468, 468, 468,   0,
       29, 468, 468, 468, 468, 468, 468, 468, 468, 468,
      468, 468, 468, 468, 468, 180, 240, 468,  95,   5,
      240,  50,  84,   0, 170,  95, 155,  19, 200, 250,
      215,   4, 195,  80, 114, 180,  45, 190, 189,  60,
      468, 255, 468, 468, 468, 468, 468, 144,   9,   4,
       25, 135,  75,  14,   9, 180, 468,   4, 165,  14,
      119,  80, 245,  55, 195,  10,  65, 220,  19, 125,
       40,   5,   0, 468, 468, 468, 468, 468, 468, 468,
      468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
      468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
      468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
      468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
      468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
      468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
      468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
      468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
      468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
      468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
      468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
      468, 468, 468, 468, 468, 468, 468, 468, 468, 468,
      468, 468, 468, 468, 468, 468, 468, 468, 468
    };
  return len + asso_values[static_cast<unsigned char>(str[len - 1])] + asso_values[static_cast<unsigned char>(str[0]+3)];
}

struct Requests *
Perfect_Hash::in_word_set (const char *str, size_t len)
{
  static struct Requests wordlist[] =
    {
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 34 "requests.gperf"
      {"Fc", REQ_Fc},
#line 39 "requests.gperf"
      {"BI", REQ_BI},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 80 "requests.gperf"
      {"Bk", REQ_Bk},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 12 "requests.gperf"
      {"ec", REQ_ec},
#line 124 "requests.gperf"
      {"Ns", REQ_Ns},
#line 76 "requests.gperf"
      {"de1", REQ_de1},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 112 "requests.gperf"
      {"Nm", REQ_Nm},
      {(char*)0, REQ_UNKNOWN},
#line 23 "requests.gperf"
      {"ps", REQ_ps},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 6 "requests.gperf"
      {"ds", REQ_ds},
#line 31 "requests.gperf"
      {"Fd", REQ_Fd},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 111 "requests.gperf"
      {"Nd", REQ_Nd},
#line 92 "requests.gperf"
      {"Bd", REQ_Bd},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 4 "requests.gperf"
      {"ab", REQ_ab},
#line 7 "requests.gperf"
      {"as", REQ_as},
#line 145 "requests.gperf"
      {"als", REQ_als},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 74 "requests.gperf"
      {"am", REQ_am},
#line 86 "requests.gperf"
      {"Fx", REQ_Fx},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 87 "requests.gperf"
      {"Nx", REQ_Nx},
#line 89 "requests.gperf"
      {"Bx", REQ_Bx},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 14 "requests.gperf"
      {"ex", REQ_ex},
#line 118 "requests.gperf"
      {"Sy", REQ_Sy},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 62 "requests.gperf"
      {"Sh", REQ_Sh},
#line 59 "requests.gperf"
      {"Ss", REQ_Ss},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 157 "requests.gperf"
      {"Sm", REQ_Sm},
#line 140 "requests.gperf"
      {"Bq", REQ_Bq},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 58 "requests.gperf"
      {"SM", REQ_SM},
#line 36 "requests.gperf"
      {"Ft", REQ_Ft},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 119 "requests.gperf"
      {"Dv", REQ_Dv},
#line 84 "requests.gperf"
      {"Bt", REQ_Bt},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 82 "requests.gperf"
      {"Dd", REQ_Dd},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 10 "requests.gperf"
      {"cc", REQ_cc},
#line 33 "requests.gperf"
      {"Fo", REQ_Fo},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 123 "requests.gperf"
      {"No", REQ_No},
#line 64 "requests.gperf"
      {"Sx", REQ_Sx},
      {(char*)0, REQ_UNKNOWN},
#line 149 "requests.gperf"
      {"shift", REQ_shift},
#line 81 "requests.gperf"
      {"Ek", REQ_Ek},
#line 13 "requests.gperf"
      {"eo", REQ_eo},
#line 152 "requests.gperf"
      {"Dx", REQ_Dx},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 151 "requests.gperf"
      {"do", REQ_do},
#line 71 "requests.gperf"
      {"nx", REQ_nx},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 109 "requests.gperf"
      {"Em", REQ_Em},
#line 25 "requests.gperf"
      {"so", REQ_so},
#line 106 "requests.gperf"
      {"Sq", REQ_Sq},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 120 "requests.gperf"
      {"Ev", REQ_Ev},
#line 9 "requests.gperf"
      {"c2", REQ_c2},
#line 100 "requests.gperf"
      {"Dq", REQ_Dq},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 93 "requests.gperf"
      {"Ed", REQ_Ed},
#line 60 "requests.gperf"
      {"St", REQ_St},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 66 "requests.gperf"
      {"Dt", REQ_Dt},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 32 "requests.gperf"
      {"Fn", REQ_Fn},
#line 108 "requests.gperf"
      {"Ad", REQ_Ad},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 61 "requests.gperf"
      {"SS", REQ_SS},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 63 "requests.gperf"
      {"SH", REQ_SH},
#line 22 "requests.gperf"
      {"nf", REQ_nf},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 144 "requests.gperf"
      {"nroff", REQ_nroff},
      {(char*)0, REQ_UNKNOWN},
#line 104 "requests.gperf"
      {"Pq", REQ_Pq},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 150 "requests.gperf"
      {"while", REQ_while},
#line 28 "requests.gperf"
      {"tm", REQ_tm},
#line 94 "requests.gperf"
      {"Be", REQ_Be},
      {(char*)0, REQ_UNKNOWN},
#line 154 "requests.gperf"
      {"break", REQ_break},
      {(char*)0, REQ_UNKNOWN},
#line 37 "requests.gperf"
      {"Fa", REQ_Fa},
#line 148 "requests.gperf"
      {"aln", REQ_aln},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 75 "requests.gperf"
      {"de", REQ_de},
#line 139 "requests.gperf"
      {"Aq", REQ_Aq},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 30 "requests.gperf"
      {"I", REQ_I},
#line 98 "requests.gperf"
      {"Pf", REQ_Pf},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 115 "requests.gperf"
      {"Ic", REQ_Ic},
#line 85 "requests.gperf"
      {"At", REQ_At},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 44 "requests.gperf"
      {"DT", REQ_DT},
#line 96 "requests.gperf"
      {"Fl", REQ_Fl},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 141 "requests.gperf"
      {"Qq", REQ_Qq},
#line 77 "requests.gperf"
      {"Bl", REQ_Bl},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 18 "requests.gperf"
      {"el", REQ_el},
#line 52 "requests.gperf"
      {"PD", REQ_PD},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 21 "requests.gperf"
      {"ig", REQ_ig},
#line 43 "requests.gperf"
      {"RI", REQ_RI},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 15 "requests.gperf"
      {"fc", REQ_fc},
#line 56 "requests.gperf"
      {"RE", REQ_RE},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 132 "requests.gperf"
      {"%Q", REQ_perc_Q},
#line 53 "requests.gperf"
      {"Rs", REQ_Rs},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 5 "requests.gperf"
      {"di", REQ_di},
#line 121 "requests.gperf"
      {"Fr", REQ_Fr},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 103 "requests.gperf"
      {"Oc", REQ_Oc},
#line 38 "requests.gperf"
      {"BR", REQ_BR},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 143 "requests.gperf"
      {"troff", REQ_troff},
#line 110 "requests.gperf"
      {"Va", REQ_Va},
#line 83 "requests.gperf"
      {"Os", REQ_Os},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 11 "requests.gperf"
      {"ce", REQ_ce},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 138 "requests.gperf"
      {"An", REQ_An},
#line 91 "requests.gperf"
      {"Dl", REQ_Dl},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 79 "requests.gperf"
      {"It", REQ_It},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 97 "requests.gperf"
      {"Pa", REQ_Pa},
#line 116 "requests.gperf"
      {"Ms", REQ_Ms},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 90 "requests.gperf"
      {"Ux", REQ_Ux},
#line 133 "requests.gperf"
      {"%V", REQ_perc_V},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 69 "requests.gperf"
      {"rm", REQ_rm},
#line 88 "requests.gperf"
      {"Ox", REQ_Ox},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 20 "requests.gperf"
      {"if", REQ_if},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 29 "requests.gperf"
      {"B", REQ_B},
#line 17 "requests.gperf"
      {"ft", REQ_ft},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 78 "requests.gperf"
      {"El", REQ_El},
#line 73 "requests.gperf"
      {"nr", REQ_nr},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 114 "requests.gperf"
      {"Cm", REQ_Cm},
#line 126 "requests.gperf"
      {"nN", REQ_nN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 159 "requests.gperf"
      {"Xc", REQ_Xc},
#line 54 "requests.gperf"
      {"RS", REQ_RS},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 24 "requests.gperf"
      {"sp", REQ_sp},
#line 113 "requests.gperf"
      {"Cd", REQ_Cd},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 26 "requests.gperf"
      {"ta", REQ_ta},
#line 65 "requests.gperf"
      {"TS", REQ_TS},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 67 "requests.gperf"
      {"TH", REQ_TH},
#line 102 "requests.gperf"
      {"Oo", REQ_Oo},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 105 "requests.gperf"
      {"Ql", REQ_Ql},
#line 128 "requests.gperf"
      {"%D", REQ_perc_D},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 72 "requests.gperf"
      {"in", REQ_in},
#line 57 "requests.gperf"
      {"SB", REQ_SB},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 107 "requests.gperf"
      {"Ar", REQ_Ar},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 48 "requests.gperf"
      {"P", REQ_P},
#line 50 "requests.gperf"
      {"PP", REQ_PP},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 137 "requests.gperf"
      {"%T", REQ_perc_T},
#line 19 "requests.gperf"
      {"ie", REQ_ie},
#line 155 "requests.gperf"
      {"nop", REQ_nop},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 27 "requests.gperf"
      {"ti", REQ_ti},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 125 "requests.gperf"
      {"Tn", REQ_Tn},
#line 51 "requests.gperf"
      {"HP", REQ_HP},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 55 "requests.gperf"
      {"Re", REQ_Re},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 142 "requests.gperf"
      {"tr", REQ_tr},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 99 "requests.gperf"
      {"Pp", REQ_Pp},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 8 "requests.gperf"
      {"br", REQ_br},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 153 "requests.gperf"
      {"Ta", REQ_Ta},
#line 158 "requests.gperf"
      {"Xo", REQ_Xo},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 70 "requests.gperf"
      {"rn", REQ_rn},
#line 147 "requests.gperf"
      {"rnn", REQ_rnn},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 47 "requests.gperf"
      {"IX", REQ_IX},
#line 156 "requests.gperf"
      {"URL", REQ_URL},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 41 "requests.gperf"
      {"IR", REQ_IR},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 135 "requests.gperf"
      {"%J", REQ_perc_J},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 16 "requests.gperf"
      {"fi", REQ_fi},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 127 "requests.gperf"
      {"%A", REQ_perc_A},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 45 "requests.gperf"
      {"IP", REQ_IP},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 68 "requests.gperf"
      {"TX", REQ_TX},
#line 136 "requests.gperf"
      {"%R", REQ_perc_R},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 129 "requests.gperf"
      {"%N", REQ_perc_N},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 117 "requests.gperf"
      {"Or", REQ_Or},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 40 "requests.gperf"
      {"IB", REQ_IB},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 131 "requests.gperf"
      {"%P", REQ_perc_P},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 46 "requests.gperf"
      {"TP", REQ_TP},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 35 "requests.gperf"
      {"OP", REQ_OP},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 146 "requests.gperf"
      {"rr", REQ_rr},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 42 "requests.gperf"
      {"RB", REQ_RB},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 134 "requests.gperf"
      {"%B", REQ_perc_B},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 122 "requests.gperf"
      {"Li", REQ_Li},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 130 "requests.gperf"
      {"%O", REQ_perc_O},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 101 "requests.gperf"
      {"Op", REQ_Op},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN},
#line 95 "requests.gperf"
      {"Xr", REQ_Xr},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
      {(char*)0, REQ_UNKNOWN}, {(char*)0, REQ_UNKNOWN},
#line 49 "requests.gperf"
      {"LP", REQ_LP}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          const char *s = wordlist[key].name;

          if (s && *str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
            return &wordlist[key];
        }
    }
  return 0;
}
