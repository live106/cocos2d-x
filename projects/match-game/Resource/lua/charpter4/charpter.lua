--[[
Name���ؿ����ƣ���ʵ������
Type����Ϸ���ͣ�0Ϊ�Ӽ�����Ϸ��1Ϊ������Ϸ���ؿ�����󽫸������ֵѡ����Ӧͨ���㷨
Timelimit����Ϸ��ʱ��Ϊ0ʱ��ʾ����ʱ����λΪ��
Allowtomovenum�������û��ƶ��Ļ����Ŀ
slots�����õ�Ϊ��Ϸ�л��ĳ�ʼ�ڷŹ���֧�ֶ������͵İڷţ���type�ַ������֣�
     type="number":ͨ�����ڳ���һ�����֣����֧��4��eight��eight���Ƿ��ʼ�����ڻ��ͨ���ڲ��ĳ���Ϊ7��bool���鹹�ɣ���ر�����£�
                             -2-
                            |   |      
                            1   3
                            |   |
                             -4-
                            |   |
                            5   6
                             -7-
           ����������������������֣�
           0={true, true, true, false, true, true, true}
           1={false, false,true,false,false,true,false}
           2={false, true, true, true, true, false, true}
           3={false, true, true, true, false, true, true}
           4={true, false, true, true, false, true, false}
           5={true, true, false, true, false, true, true}
           6={true, true, false, true, true, true, true}
           7={false, true, true, false, false, true, false}
           8={true, true, true, true, true, true, true}
           9={true, true, true, true, false, true, true}

     type="operator"��ͨ�����ڷų��������
     type="equalsign"��ͨ�����ڷų��Ⱥ�
     type="rectangle3x3"��ͨ�����ڷų�3��3�ľ���
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
				{true, true, true, false, true, true, true}
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
				{true, true, true, true, true, true, true}
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
				{false, false,true,false,false,true,false},
                {true, false, true, true, false, true, false}
			}
		}
	}
}