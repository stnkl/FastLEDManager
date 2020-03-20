#include "gamma_correction_table.h"

// This table was created using the following code:
//
// #include <iostream>
// #include <cmath>
// using namespace std;

// int main()
// {
//     int max_input = 255; // 8 bit
//     int max_output = 4095; // 12 bit
//     float gamma = 2;

// 	cout << "const uint16_t PROGMEM gamma_correction_table[] = {";
// 	for(float i = 0; i <= max_input; i++)
// 	{
// 		cout << endl << "\t" << roundf( pow(i,gamma) / ( pow(max_input,gamma) / max_output ) );
// 		if(i < max_input)
// 		    cout << ",";
// 	    cout << " // " << i;
// 	}
// 	cout << "\b\b \n};" << endl;
// }

const uint32_t gamma_correction_table[] PROGMEM = {
	0, // 0
	0, // 1
	0, // 2
	1, // 3
	1, // 4
	2, // 5
	2, // 6
	3, // 7
	4, // 8
	5, // 9
	6, // 10
	8, // 11
	9, // 12
	11, // 13
	12, // 14
	14, // 15
	16, // 16
	18, // 17
	20, // 18
	23, // 19
	25, // 20
	28, // 21
	30, // 22
	33, // 23
	36, // 24
	39, // 25
	43, // 26
	46, // 27
	49, // 28
	53, // 29
	57, // 30
	61, // 31
	64, // 32
	69, // 33
	73, // 34
	77, // 35
	82, // 36
	86, // 37
	91, // 38
	96, // 39
	101, // 40
	106, // 41
	111, // 42
	116, // 43
	122, // 44
	128, // 45
	133, // 46
	139, // 47
	145, // 48
	151, // 49
	157, // 50
	164, // 51
	170, // 52
	177, // 53
	184, // 54
	191, // 55
	197, // 56
	205, // 57
	212, // 58
	219, // 59
	227, // 60
	234, // 61
	242, // 62
	250, // 63
	258, // 64
	266, // 65
	274, // 66
	283, // 67
	291, // 68
	300, // 69
	309, // 70
	317, // 71
	326, // 72
	336, // 73
	345, // 74
	354, // 75
	364, // 76
	373, // 77
	383, // 78
	393, // 79
	403, // 80
	413, // 81
	423, // 82
	434, // 83
	444, // 84
	455, // 85
	466, // 86
	477, // 87
	488, // 88
	499, // 89
	510, // 90
	522, // 91
	533, // 92
	545, // 93
	556, // 94
	568, // 95
	580, // 96
	593, // 97
	605, // 98
	617, // 99
	630, // 100
	642, // 101
	655, // 102
	668, // 103
	681, // 104
	694, // 105
	708, // 106
	721, // 107
	735, // 108
	748, // 109
	762, // 110
	776, // 111
	790, // 112
	804, // 113
	818, // 114
	833, // 115
	847, // 116
	862, // 117
	877, // 118
	892, // 119
	907, // 120
	922, // 121
	937, // 122
	953, // 123
	968, // 124
	984, // 125
	1000, // 126
	1016, // 127
	1032, // 128
	1048, // 129
	1064, // 130
	1081, // 131
	1097, // 132
	1114, // 133
	1131, // 134
	1148, // 135
	1165, // 136
	1182, // 137
	1199, // 138
	1217, // 139
	1234, // 140
	1252, // 141
	1270, // 142
	1288, // 143
	1306, // 144
	1324, // 145
	1342, // 146
	1361, // 147
	1379, // 148
	1398, // 149
	1417, // 150
	1436, // 151
	1455, // 152
	1474, // 153
	1494, // 154
	1513, // 155
	1533, // 156
	1552, // 157
	1572, // 158
	1592, // 159
	1612, // 160
	1632, // 161
	1653, // 162
	1673, // 163
	1694, // 164
	1715, // 165
	1735, // 166
	1756, // 167
	1777, // 168
	1799, // 169
	1820, // 170
	1841, // 171
	1863, // 172
	1885, // 173
	1907, // 174
	1929, // 175
	1951, // 176
	1973, // 177
	1995, // 178
	2018, // 179
	2040, // 180
	2063, // 181
	2086, // 182
	2109, // 183
	2132, // 184
	2155, // 185
	2179, // 186
	2202, // 187
	2226, // 188
	2250, // 189
	2273, // 190
	2297, // 191
	2322, // 192
	2346, // 193
	2370, // 194
	2395, // 195
	2419, // 196
	2444, // 197
	2469, // 198
	2494, // 199
	2519, // 200
	2544, // 201
	2570, // 202
	2595, // 203
	2621, // 204
	2647, // 205
	2672, // 206
	2698, // 207
	2725, // 208
	2751, // 209
	2777, // 210
	2804, // 211
	2830, // 212
	2857, // 213
	2884, // 214
	2911, // 215
	2938, // 216
	2965, // 217
	2993, // 218
	3020, // 219
	3048, // 220
	3076, // 221
	3104, // 222
	3132, // 223
	3160, // 224
	3188, // 225
	3217, // 226
	3245, // 227
	3274, // 228
	3303, // 229
	3331, // 230
	3360, // 231
	3390, // 232
	3419, // 233
	3448, // 234
	3478, // 235
	3507, // 236
	3537, // 237
	3567, // 238
	3597, // 239
	3627, // 240
	3658, // 241
	3688, // 242
	3719, // 243
	3749, // 244
	3780, // 245
	3811, // 246
	3842, // 247
	3873, // 248
	3905, // 249
	3936, // 250
	3968, // 251
	3999, // 252
	4031, // 253
	4063, // 254
	4095 // 255
};