/**
  *
  * @file       read.h 
  * @author     Z B T
  * @version    V0.1
  * @date       2016.05.01
  * @brief      项目工程说明
  *
  */  


/**< -------------------------- Update Log ------------------------------------- */
/**<

Date： 2016.05.01
    1. 新增统一设置小球移动速度函数
    
    2.修改为小游戏 
        增加挡板 按键调节其位置
        若小球运动过程中没碰到挡板则继续下移触碰到屏幕物理边界后消失 代表此局结束
        Do not worry 一会小球会回到屏幕的 然后可以继续无聊的玩下一局啦o(╯□╰)o
        吃午饭去╭(╯^╰)╮
Date: 2016.05.03
    3. 修复在某些情况下小球在挡板所在边界线（和挡板在同一水平线但是小球没碰到挡板）依然会反弹回去的Bug  
    4. 完善游戏 新增游戏开始确认 、游戏进行 、游戏结束画面, 阔以实时显示当前得分情况，碰到挡板一次得一分
       没碰到挡板就会Game over 
       就酱 

*/

