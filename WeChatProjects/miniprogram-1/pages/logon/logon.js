// pages/logon.js
Page({

  /**
   * 页面的初始数据
   */
  data: {
    username:'',//用户名
    password:'',//密码
    email:'',//邮箱
    Message:'',//服务器返回信息
    dataObject:{},//数据的格式
  },

  //处理获取的表单信息
  submitForm:function(e){
    const that=this
    const formData=e.detail.value;
    //getApp().globalData.userInfo=formData
    const { username, password, email } = formData;  
  
    // 检查用户名是否为空  
    if (!username) {  
      wx.showToast({  
        title: '用户名不能为空',  
        icon: 'none',  
        duration: 2000,  
      });  
      return;  
    }  
  
    // 检查密码是否为空  
    if (!password) {  
      wx.showToast({  
        title: '密码不能为空',  
        icon: 'none',  
        duration: 2000,  
      });  
      return;  
    }  
  
    // // 检查邮箱是否为空  
    // if (!email) {  
    //   wx.showToast({  
    //     title: '邮箱不能为空',  
    //     icon: 'none',  
    //     duration: 2000,  
    //   });  
    //   return;  
    // }  
  
    // // 检查邮箱格式是否正确  
    // const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;  
    // if (!emailRegex.test(email)) {  
    //   wx.showToast({  
    //     title: '请输入正确的邮箱地址',  
    //     icon: 'none',  
    //     duration: 2000,  
    //   });  
    //   return;  
    // }
    that.setData({
      username:formData.username,
      password:formData.password,
      //email:formData.email,
    });

    let dataDictionary={
      "status":{
        "client":0,
        "error":0
    },
    "information":{
      "username":this.data.username,
      "password":this.data.password,
      //"email":this.data.email
    },
    };
    that.setData({
      dataObject:dataDictionary
    })

    //转换为arraybuffer格式
    //先转换为字符串
    const jsonString=JSON.stringify(that.data.dataObject);
    // 创建一个TextEncoder实例  
    // let encoding_index = require("encoding-indexes.js");
    // let encoding=require("encoding.js")
    let u8array = new TextEncoder('utf-8').encode(jsonString)
    that.CreateTCP(u8array,formData);
  },



  //创建creatTcpSocket连接
  CreateTCP:function(arrayBuffer,UserData){
    if(getApp().globalData.tcpSocket==""){
      console.log(getApp().globalData.tcpSocket)
        getApp().globalData.tcpSocket=wx.createTCPSocket()
        getApp().globalData.tcpSocket.connect({
          address:getApp().globalData.address,
          //address:'127.0.0.1',
          port:'50002'
        });
        //console.log(getApp().globalData.tcpSocket)
      }
    let tcpSocket=getApp().globalData.tcpSocket

    //监听打开事件
    //tcpSocket.onConnect();

    //监听失败事件
    tcpSocket.onError(function (res) {
      console.log('连接服务器失败：'+res.errMsg);
    });
    const that=this;
    var data=new Uint8Array(0);
    //接收服务器返回信息
    tcpSocket.onMessage(function (res) {
      const result=res.message;
      const view=new Uint8Array(result);
      let length=data.length
      data=new Uint8Array(data.length+view.length)
      data.set(data)
      data.set(view,length)
      // let decoding_index = require("../encoding-indexes.js");
      // let decoding=require("encoding.js")
      const string=new TextDecoder('utf-8').decode(data)
      const stringJson=JSON.parse(string);
      if(stringJson.status.error===1){
        const errString=stringJson.response.data;
        wx.showToast({
          title: errString,
          icon:'error'
        })
        console.log(errString)
      }else{
        let email=stringJson.response.data;
        getApp().globalData.userInfo=UserData
        getApp().globalData.userInfo.email=email
        wx.showToast({
          title: '登录成功',
          icon:'success'
        })
      }
    })
    tcpSocket.write(arrayBuffer)//发送信息
  },




  /**
   * 生命周期函数--监听页面加载
   */
  onLoad(options) {

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