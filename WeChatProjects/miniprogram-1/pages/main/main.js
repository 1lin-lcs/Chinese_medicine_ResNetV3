// pages/main/main.js

  //选择图片并上传服务器
  Page({  
    data:{
      base64Images:'',////存储转换为base64编码的图片
      images:'',//存储用户上传的图片路径
      lastnames:'',//文件后缀名存储数组
      dataObject:{},//数据的格式
      arraybufferObject:null,//上传的数据
      Name:'',//中草药名字
      Function:'',//中草药功效
      comment_list:[],
      hidden1:true,
      hidden2:true,
      isComment:"评论",
      external:-1,
      inputMessage:""
    },  

    chooseImages:function(){
      const that=this;
      wx.chooseMedia({
        count:1,//图片最大上传数
        sizeType: ['original', 'compressed'], // 可以指定是原图还是压缩图，默认二者都有  
        sourceType: ['album', 'camera'], // 可以指定来源是相册还是相机，默认二者都有
        success(res){
          that.setData({
            images:res.tempFiles[0].tempFilePath,
            lastnames:'.'+res.tempFiles[0].tempFilePath.split(".")[1],
          }),

          //转换为base64编码
          wx.getFileSystemManager().readFile({
            filePath:that.data.images,
            encoding:'base64',
            success(res){
              that.setData({
                base64Images: res.data
              })


             //创建格式  
            let dataDictionary={
              "status":{
                "client":4,
                "error":0
            },
            "response":{
                "data":that.data.base64Images,
                "external":that.data.lastnames
            }
            };
            that.setData({
              dataObject:dataDictionary,
            })

            //转换为arraybuffer格式
            //先转换为字符串
            const jsonString=JSON.stringify(that.data.dataObject);
            // 创建一个TextEncoder实例  
            // let encoding_index = require("encoding-indexes.js");
            // let encoding=require("encoding.js")
            let u8array = new TextEncoder('utf-8').encode(jsonString)
            that.CreateTCP(u8array);
            },
            fail(res){
              console.log('转换为base64编码失败')
            }
          });
        },
        fail(res){
          //console.log('获取图片失败')
        },
      })
    },

    
    //创建creatTcpSocket连接
    CreateTCP:function(arraybuffer){
      //console.log(1)
      //const tcpSocket=wx.createTCPSocket();
      if(getApp().globalData.tcpSocket==""){
        //console.log(getApp().globalData.tcpSocket)
        getApp().globalData.tcpSocket=wx.createTCPSocket()
        getApp().globalData.tcpSocket.connect({
            address:getApp().globalData.address,
            //address:'127.0.0.1',
            port:'50002'
        });
        //console.log(getApp().globalData.tcpSocket)
      }
      let tcpSocket=getApp().globalData.tcpSocket

      var offset=0
      var chunk=0
      var isUpload=false
      //监听打开事件
      //tcpSocket.onConnect();

      //监听失败事件
      tcpSocket.onError(function (res) {
        if(isUpload){
          offset-=chunk.byteLength;
          //console.log('连接服务器失败：'+res.errMsg+"\n错误偏移："+offset);
        }
        else{
          console.log('连接服务器失败：'+res.errMsg);
          // if(res.errMsg.indexOf("[ERR_SOCKET_CLOSED]",0)!=-1){
          //   getApp().globalData.tcpSocket=wx.createTCPSocket()
          //   getApp().globalData.tcpSocket.connect({
          //     address:getApp().globalData.address,
          //     port:'50002'
          //   });}
          //console.log(getApp().globalData.tcpSocket)
          //console.log(res)
        }
        
      });
      tcpSocket.onClose(function(res){
        console.log(res)
        console.log("关闭")
      })
      const that=this;
      var data=new Uint8Array(0);
      //接收服务器返回信息
      tcpSocket.onMessage(function (res) {
        const result=res.message;
        //console.log(result)
        const view=new Uint8Array(result);
        let length=data.length
        data=new Uint8Array(data.length+view.length)
        data.set(data)
        data.set(view,length)
        //console.log(data)
        // let decoding_index = require("encoding-indexes.js");
        // let decoding=require("encoding.js")
        const string=new TextDecoder('utf-8').decode(data)
        //console.log("string is "+string)
        const stringJson=JSON.parse(string);
        if(stringJson.status.error===1){
          const errString=stringJson.response.data;
          wx.showToast({
            title: errString,
            icon:"error"
          })
        }else{
          let list=[]
          let answer=""
          let isHidden=false
          //console.log(stringJson)
          //console.log(stringJson.response.external)
          if(stringJson.response.hasOwnProperty('comment')){
            answer="评论"
            //console.log(stringJson.response.comment)
              for(var i=0;i<stringJson.response.comment.length;i++){
                // console.log(stringJson.response.comment[i])
                list.push(stringJson.response.comment[i])
              }
              //console.log(list)
          }
          else{
            answer="暂无评论"
            isHidden=true
          }
          that.setData({
            Name:stringJson.response.data,
            Function:stringJson.response.external[1],
            comment_list:list,
            external:stringJson.response.external[stringJson.response.external.length-1],
            hidden1:false,
            hidden2:isHidden,
            isComment:answer
          })
        }
      })
      isUpload=true
        //console.log(arraybuffer)
      const size=1024
        
      while(offset<arraybuffer.byteLength){
        chunk = arraybuffer.slice(offset, Math.min(arraybuffer.byteLength, offset + size));
        offset += chunk.byteLength;
        tcpSocket.write(chunk)
          //console.log("已发送："+offset)
      }
      offset=0
      isUpload=false
        //tcpSocket.write(arraybuffer)
    },
    
    submitForm:function(e){
      if(getApp().globalData.userInfo==null){
        wx.showToast({
          title: '请登录',
          icon:"error"
        })
        return;
      }
      const that=this
      let dataDictionary={
        "status":{
          "client":5,
          "error":0
        },
        "response":{
          data:e.detail.value.comment,
          external:that.data.external
        },
        "information":{
          "username":getApp().globalData.userInfo.username
        }
      };
      that.setData({
        dataObject:dataDictionary
      })
      let newCommend={
        "comment":e.detail.value.comment,
        "name":getApp().globalData.userInfo.username
      }
      //转换为arraybuffer格式
      //先转换为字符串
      const jsonString=JSON.stringify(that.data.dataObject);
      // 创建一个TextEncoder实例  
      // let encoding_index = require("./encoding-indexes.js");
      // let encoding=require("encoding.js")
      let u8array = new TextEncoder('utf-8').encode(jsonString)
      that.CreateTCP1(u8array,newCommend);
    },
    CreateTCP1:function(arrayBuffer,newCommend){
      if(getApp().globalData.tcpSocket==""){
        //console.log(getApp().globalData.tcpSocket)
        getApp().globalData.tcpSocket=wx.createTCPSocket()
        getApp().globalData.tcpSocket.connect({
          address:getApp().globalData.address,
          //address:'127.0.0.1',
          port:'50002'
        });
      }
      let tcpSocket=getApp().globalData.tcpSocket
      //console.log(newCommend)
      //console.log(this.data.comment_list.concat(newCommend))
      //监听打开事件
      // tcpSocket.onConnect(function (res) {
      //   tcpSocket.write(arrayBuffer)//发送信息
      // });
  
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
        // let decoding_index = require("encoding-indexes.js");
        // let decoding=require("encoding.js")
        const string=new TextDecoder('utf-8').decode(data)
        //console.log(string)
        const stringJson=JSON.parse(string);
        if(stringJson.status.error===1){
          const errString=stringJson.response.data;
          wx.showToast({
            title: '服务器出错',
            icon:"error"
          })
          console.log(errString)
        }else{
          wx.showToast({
            title: '发布成功',
            icon:"success"
          })
          that.setData({
            comment_list:that.data.comment_list.concat(newCommend),
            isComment:"评论",
            inputMessage:""
          })
        }
      })
      tcpSocket.write(arrayBuffer)//发送信息
    },
    

    onLoad: function() {  
      // 页面加载时尝试建立 WebSocket 连接  
      this.CreateTCP();  
    },  
    
    onUnload: function() {  
      // 页面卸载时关闭 WebSocket 连接  
      this.CloseTCP();  
    },
    // 关闭 TCP Socket 连接  
    CloseTCP: function() {  
      wx.closeSocket();  
    },
   
    
  /**
   * 生命周期函数--监听页面加载
   */
  onLoad(options) {
    if(getApp().globalData.tcpSocket==""){
      //console.log(getApp().globalData.tcpSocket)
      getApp().globalData.tcpSocket=wx.createTCPSocket()
      getApp().globalData.tcpSocket.connect({
        address:getApp().globalData.address,
        port:'50002'
      });
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
