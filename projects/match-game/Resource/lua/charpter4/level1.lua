--[[
Name：关卡名称，无实际意义
Type：游戏类型，0为加减法游戏，1为矩阵游戏，关卡载入后将根据这个值选择相应通关算法
Timelimit：游戏限时，为0时表示不限时，单位为秒
Allowtomovenum：允许用户移动的火柴数目
slots下设置的为游戏中火柴的初始摆放规则，1为摆放火柴，0为不摆放，将按照从上到下，从左到右方式摆放出3×3矩阵
]]
level=
{
	name="level1",
	type=1,
	timelimit=60,
	--[[//去掉的火柴数]]
	Allowtomovenum = 2,
	slots=
	{
		1,1,1,
		1,1,1,1,
		1,1,1,
		1,1,1,1,
		1,1,1,
		1,1,1,1,
		1,1,1
	},
    --[[ //所有正方形]]
	normalsquares = 7,
    --[[//大正方形]]
	bigsquare = 0, 
    --[[//中正方形]]
	mediumsquare = 0,
    --[[//小正方形]] 
	smallsquare = 0,
    --[[//相同的正方形]] 
	samesquare = 0, 
}