// pages/personalMessage.js
var logonData=require('../logon/logon.js')
Page({

  /**
   * 页面的初始数据
   */
  data: {
    username:'',//用户名
    password:'',//密码
    email:'',//邮箱
    receivedUserInfo:''
  },



  /**
   * 生命周期函数--监听页面加载
   */
  onLoad:function(){
    if(getApp().globalData.userInfo==null){
      wx.showToast({
        title: '未登录',
        icon:'error'
      })
      return
    }
    this.setData({
      username:getApp().globalData.userInfo.username,
      password:getApp().globalData.userInfo.password,
      email:getApp().globalData.userInfo.email
    })
  },
  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady() {
    
  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow() {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide() {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload() {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh() {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom() {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage() {

  }
})