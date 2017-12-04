#ifndef Engine_Enum_h__
#define Engine_Enum_h__

enum eSceneID 
{
	SceneID_Logo,
	SceneID_Stage,
	SceneID_End
};

enum eBuffer_Type
{
	Buffer_RcTex,
	Buffer_RcTerrain,
	Buffer_CubeTex,
	Buffer_CubeCol,
	Buffer_End
};

enum eTexture_Type
{
	Texture_Normal,
	Texture_Cube,
	Texture_End
};

enum eLayerID
{
	Layer_Environment,
	Layer_GameLogic,
	Layer_Camera,
	Layer_UI,
	Layer_End
};

enum eAngle
{
	Angle_X,
	Angle_Y,
	Angle_Z,
	Angle_End
};

enum eResource_Type
{
	Resource_Logo,
	Resource_Stage,
	Resource_Static,
	Resource_End
};

enum eMouseClick
{
	DIM_Lbutton,
	DIM_RButton,
	DIM_MButton
};

enum eMouseMove
{
	DIM_X,
	DIM_Y,
	DIM_Z
};

enum eLoadingID
{
	Load_Stage,
	Load_Stage1,
	Load_Stage2,
	Load_End
};

enum eMeshType
{
	Mesh_Static,
	Mesh_Dynamic,
	Mesh_End
};

enum ePoint
{
	Point_A,
	Point_B,
	Point_C,
	Point_END
};

enum eLine
{
	Line_AB,
	Line_BC,
	Line_CA,
	Line_END
};

enum eNeighbor
{
	Neighbor_AB,
	Neighbor_BC,
	Neighbor_CA,
	Neighbor_END
};

enum eCustomColor
{
	CustomColor_R,
	CustomColor_G,
	CustomColor_B,
	CustomColor_END,	
};

enum ePlayerAniIndex
{
	PlayerAniIndex_Idle,
	PlayerAniIndex_Move,
	PlayerAniIndex_Run,
	PlayerAniIndex_RunCharge_Begin,
	PlayerAniIndex_RunCharge_Loop,
	PlayerAniIndex_RunCharge_End,
	PlayerAniIndex_RunStop,
	PlayerAniIndex_SkillCharge1,
	PlayerAniIndex_SkillCharge2,
	PlayerAniIndex_SkillCharge3,
	PlayerAniIndex_SkillCharge4,
	PlayerAniIndex_SkillCharge5,
	PlayerAniIndex_Damage_Front,
	PlayerAniIndex_Damage_Back,
	PlayerAniIndex_BigDamege_Front,
	PlayerAniIndex_BigDamege_Front_End,
	PlayerAniIndex_BigDamege_Back,
	PlayerAniIndex_BigDamege_Back_End,
	PlayerAniIndex_Dash_N,
	PlayerAniIndex_Dash_NE,
	PlayerAniIndex_Dash_E,
	PlayerAniIndex_Dash_SE,
	PlayerAniIndex_Dash_S,
	PlayerAniIndex_Dash_SW,
	PlayerAniIndex_Dash_W,
	PlayerAniIndex_Dash_NW,
	PlayerAniIndex_Magic_MagicArrow,
	PlayerAniIndex_Magic_FireBolt,
	PlayerAniIndex_Magic_FireShock,
	//PlayerAniIndex_Magic_FireStorm,
	//PlayerAniIndex_Magic_IceBlast,
	PlayerAniIndex_Magic_IceBlow,
	PlayerAniIndex_Magic_IceSpaer,
	//PlayerAniIndex_Magic_Rage_Begin,
	//PlayerAniIndex_Magic_Rage_Loop,
	//PlayerAniIndex_Magic_Rage_End,
	PlayerAniIndex_DarkKnight_Transform1,
	//PlayerAniIndex_DarkKnight_Transform2,
	PlayerAniIndex_End
};

enum eDarkKnightAniIndex
{
	DarkKnightAniIndex_Idle,
	DarkKnightAniIndex_Move,
	DarkKnightAniIndex_Run,
	DarkKnightAniIndex_RunStop,
	DarkKnightAniIndex_Attack1,
	DarkKnightAniIndex_Attack2,
	DarkKnightAniIndex_Attack3,
	DarkKnightAniIndex_Attack4,
	DarkKnightAniIndex_StrongAttack0,
	DarkKnightAniIndex_StrongAttack0_Charge,
	DarkKnightAniIndex_StrongAttack1_a,
	DarkKnightAniIndex_StrongAttack1_b,
	DarkKnightAniIndex_StrongAttack1_c,
	DarkKnightAniIndex_StrongAttack2,
	DarkKnightAniIndex_StrongAttack3,
	DarkKnightAniIndex_StrongAttack4,
	DarkKnightAniIndex_Damege_Front,
	DarkKnightAniIndex_Damege_Back,
	DarkKnightAniIndex_BigDamege_Back_Begin,
	DarkKnightAniIndex_BigDamege_Back_End,
	DarkKnightAniIndex_BigDamege_Front_Begin,
	DarkKnightAniIndex_BigDamege_Front_End,
	DarkKnightAniIndex_Blink_N,
	DarkKnightAniIndex_Blink_S,
	DarkKnightAniIndex_Blink_Attack,
	DarkKnightAniIndex_DarkKnight_Transform,
	DarkKnightAniIndex_End
};

enum ePlayerState
{
	Player_Idle,
	Player_Move,
	Player_Dash,
	Player_DashCharge,
	Player_Charge,
	Player_Attack,
	Player_Hit,
	Player_Transform, //º¯½Å
	Player_ManaAmber,
	Player_Blink,
	Player_End
};

enum ePlayerMagic
{
	Magic_MagicArrow,
	Magic_FireBolt,
	Magic_FireShock,
	Magic_FireStome,
	Magic_IceBlast,
	Magic_IceBlow,
	Magic_Rage,
	Magic_End
};

enum eRenderType
{
	Type_Priority,
	Type_Shader,
	Type_UI,
	Type_End
};

enum eDirKey
{
	Dir_Up,
	Dir_Down,
	Dir_Left,
	Dir_Right,
	Dir_End
};

enum eKadanAniIndex
{
	KadanAniIndex_Idie,
	KadanAniIndex_Walk,
	KadanAniIndex_Run,
	KadanAniIndex_TeleportAtt1,
	KadanAniIndex_TeleportAtt2,
	KadanAniIndex_UpperAttack,
	KadanAniIndex_FourAttack,
	KadanAniIndex_Hit_Back,
	KadanAniIndex_Hit_Bottom,
	KadanAniIndex_Hit_Left,
	KadanAniIndex_Hit_Right,
	KadanAniIndex_Transform,
	KadanAniIndex_Explosion,
	KadanAniIndex_Bolt,
	KadanAniIndex_Bolt2,
	KadanAniIndex_End
};

enum eCollisionType
{
	Collision_Player,
	Collision_Kadan,
	Collision_Skill,
	Collision_End
};

enum eCustomParts
{
	CustomParts_Weapon,
	CustomParts_Upper,
	CustomParts_Lower,
	CustomParts_Foot,
	CustomParts_End
};

enum eDarkKnightType
{
	DarkKnight_None,
	DarkKnight_Staff,
	DarkKnight_Scythe,
	DarkKnight_End
};

enum eKadanState
{
	Kadan_Idle,
	Kadan_Move,
	Kadan_Dash,
	Kadan_Blink,
	Kadan_Hit,
	Kadan_Attack,
	Kadan_Transform,
	Kadan_End
};

enum eSkillType
{
	Skill_IceBlow,
	Skill_MagicArrow,
	Skill_FireShock,
	Skill_IceBlast,
	Skill_FireBolt,
	Skill_DarkHurricane,
	Skill_BlackHole,
	Skill_DarkBolt,
	Skill_DarkBolt2,
	Skill_End
};

#endif // Engine_Enum_h__