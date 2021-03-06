--[[
Name：关卡名称，无实际意义
Type：游戏类型，0为加减法游戏，1为矩阵游戏，关卡载入后将根据这个值选择相应通关算法
Timelimit：游戏限时，为0时表示不限时，单位为秒
Allowtomovenum：允许用户移动的火柴数目
slots下设置的为游戏中火柴的初始摆放规则，支持多种类型的摆放，由type字符串区分：
     type="number":通过火柴摆出的一个数字，最多支持4个eight，eight中是否初始化存在火柴通过内部的长度为7的bool数组构成，相关编号如下：
                             -2-
                            |   |      
                            1   3
                            |   |
                             -4-
                            |   |
                            5   6
                             -7-
           因此我们有如下数组表达数字：
           0={1, 1, 1, 0, 1, 1, 1}
           1={0, 0,1,0,0,1,0}
           2={0, 1, 1, 1, 1, 0, 1}
           3={0, 1, 1, 1, 0, 1, 1}
           4={1, 0, 1, 1, 0, 1, 0}
           5={1, 1, 0, 1, 0, 1, 1}
           6={1, 1, 0, 1, 1, 1, 1}
           7={0, 1, 1, 0, 0, 1, 0}
           8={1, 1, 1, 1, 1, 1, 1}
           9={1, 1, 1, 1, 0, 1, 1}

     type="operator"：通过火柴摆放出运算符，
     type="equalsign"：通过火柴摆放出等号
     type="rectangle3x3"：通过火柴摆放出3乘3的矩阵
]]
level=
{
	name="level1",
	type=0,
	timelimit=60,
	Allowtomovenum = 1,
	slots=
	{
		{
			type="number",
			eights=
			{
				{1, 1, 1, 0, 1, 1, 1}
			}
		},
		{
			type="operator",
            kind="+"
		},
		{
			type="number",
			eights=
			{
				{1, 1, 1, 1, 1, 1, 1}
			}
		},
		{
			type="operator",
			kind="="
		},
		{
			type="number",
			eights=
			{
				{0, 0,1,0,0,1,0},
        {1, 0, 1, 1, 0, 1, 0}
			}
		}
	}
}