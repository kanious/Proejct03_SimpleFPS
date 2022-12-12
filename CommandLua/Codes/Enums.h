#ifndef _ENUMS_H_
#define _ENUMS_H_

enum eSCENETAG
{
	SCENE_FPS,
	SCENE_END
};

enum eLAYERTAG
{
	LAYER_BACKGROUND,
	LAYER_OBJECT,
	LAYER_CHARACTER,
	LAYER_TARGET,
	LAYER_UI,
	LAYER_END
};

enum eOBJTAG
{
	OBJ_PLAYER,
	OBJ_BACKGROUND,
	OBJ_CAMERA,
	OBJ_DYNAMIC,
	OBJ_ITEM,
	OBJ_PROJECTILE,
	OBJ_END
};

enum CommandType
{
	Command_Serial,
	Command_Parallel,
	Command_MoveTo,
	Command_OrientTo,
	Command_FollowCurve,
	Command_FollowObject,
	Command_PlaySound,
	Command_RotSubPart,
	Command_End
};

enum EaseType
{
	Ease_Time,
	Ease_Distance,
	Ease_End
};


#endif //_ENUMS_H_