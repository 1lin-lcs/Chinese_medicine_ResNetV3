#include "IdentityThreadC++.h"

IdentityThreadCpp::IdentityThreadCpp(QString filename,QStringList categories,qintptr handle,Module module){
    m_filename=filename;
    m_categories=categories;
    m_socketDesc=handle;
    m_Module=module;
}

IdentityThreadCpp::~IdentityThreadCpp(){}

/*!
 * \brief 执行识别
 */
void IdentityThreadCpp::run(){
    cv::Mat image=cv::imread(m_filename.toLocal8Bit().toStdString());
    cv::Mat image2;
    cv::cvtColor(image,image2,cv::COLOR_BGR2RGB);
    cv::Mat image_transformed;
    cv::resize(image2,image_transformed,cv::Size(224,224));
    image_transformed.convertTo(image_transformed, CV_32FC3, 1.0 / 255.0);
    torch::Tensor tensor_image=torch::from_blob(image_transformed.data,{1,image_transformed.rows,image_transformed.cols,3});
    tensor_image=tensor_image.permute({0,3,1,2});
    tensor_image[0][0]=tensor_image[0][0].sub_(0.435).div_(0.264);
    tensor_image[0][1]=tensor_image[0][1].sub_(0.497).div_(0.253);
    tensor_image[0][2]=tensor_image[0][2].sub_(0.328).div_(0.276);
    torch::DeviceType deviceType=torch::cuda::is_available()?torch::kCUDA:torch::kCPU;
    tensor_image=tensor_image.to(deviceType);
    torch::Tensor output=m_Module.forward({tensor_image}).toTensor();
    auto prediction=output.argmax(1).item<int>();
    emit SendResult(m_categories.at(prediction),m_socketDesc);
}
