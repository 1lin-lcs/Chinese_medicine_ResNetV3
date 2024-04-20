// pages/person/person.js
Page({

  /**
   * 页面的初始数据
   */
  data: {
    disable1:false,
    disable2:false,
    disable3:false
  },

  //登录操作
  goToLogonPage:function(){
    wx.navigateTo({
      url: '/pages/logon/logon',
    })
  },


  //注册操作
  goToRegisterPage:function(){
    wx.navigateTo({
      url: '/pages/register/register',
    })
  },

  //查看个人信息操作
  personalMessage:function(){
    wx.navigateTo({
      url: '/pages/personal_message/personal_message',
    })
  },

  information:function(){
    wx.navigateTo({
      url: '/pages/information/information',
    })
  },

  Logout:function(){
    getApp().globalData.userInfo=null
    this.setData({
      disable1:false,
      disable2:false,
      disable3:true
    })
  },


  /**
   * 生命周期函数--监听页面加载
   */
  onLoad(options) {
    if(getApp().globalData.userInfo==null){
      this.setData({
        disable3:true
      })
    }
    else{
      this.setData({
        disable1:true,
        disable2:true,
        disable3:false
      })
    }
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
    if(getApp().globalData.userInfo==null){
      this.setData({
        disable3:true
      })
    }
    else{
      this.setData({
        disable1:true,
        disable2:true,
        disable3:false
      })
    }
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