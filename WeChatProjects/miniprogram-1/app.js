// app.js
import './EncoderDecoderTogether.min'
import "./utils/encoding-indexes.js"
import "./utils/encoding.js"
App({
  onLaunch() {
    // 展示本地存储能力
    const logs = wx.getStorageSync('logs') || []
    logs.unshift(Date.now())
    wx.setStorageSync('logs', logs)

    // 登录
    wx.login({
      success: res => {
        // 发送 res.code 到后台换取 openId, sessionKey, unionId
      }
    })
  },
  globalData: {
    tcpSocket:"",
    userInfo: null,
    //address:'192.168.137.1'
    address:'127.0.0.1'
  },
  // encoding_index: require("./utils/encoding-indexes.js"),
  // encoding:require("./utils/encoding.js")


  
})
