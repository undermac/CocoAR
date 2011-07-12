#ifndef __MATHLIB__
#define __MATHLIB__

//-----------------------------------------------------
#ifndef CCLOG 
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	void trace_(const char *format, ...);	
	#define CCLOG trace_
	#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	#include <android/log.h>
	#define CCLOG(...) __android_log_print(ANDROID_LOG_VERBOSE, "VEGA",__VA_ARGS__) 
	#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WOPHONE)

	#elif (CC_TARGET_PLATFORM == CC_PLATFORM_AIRPLAY)

	#else
	#error
	#endif
#endif

//-----------------------------------------------------
// WINDOWS SILLY TYPES
// [WARNING] typedef int BOOL comented... ONLY for WIN
//typedef int BOOL;
typedef unsigned long DWORD;
#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
//-----------------------------------------------------

typedef	float	vec3_t[3];	// x,y,z
typedef	float	vec4_t[4];	// x,y,z,w

#define	EPSILON	0.001f
#define	Q_PI	3.14159265358979323846f

#define DotProduct(x,y) ((x)[0] * (y)[0] + (x)[1] * (y)[1] + (x)[2] * (y)[2])

bool VectorCompare(vec3_t Vector1, vec3_t Vector2);
void CrossProduct(vec3_t Vector1, vec3_t Vector2, vec3_t Cross);
void R_ConcatTransforms(const float In1[3][4], const float In2[3][4], float Out[3][4]);
void VectorIRotate(const vec3_t In1, const float In2[3][4], vec3_t Out);
void VectorTransform(const vec3_t In1, const float In2[3][4], vec3_t Out);
void AngleQuaternion(const vec3_t Angles, vec4_t Quaternion);
void QuaternionMatrix(const vec4_t Quaternion, float (*Matrix)[4]);
void QuaternionSlerp(const vec4_t In1, vec4_t In2, float Time, vec4_t Out);

#endif
