
#include"jasonqt_vop.h"

using namespace JasonQt_Vop;

BaiduVop::BaiduVop(const QString &apiKey, const QString &secretKey):
    m_apiKey(apiKey),
    m_secretKey(secretKey)
{
    const auto &&availableDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    if(!availableDevices.isEmpty())
    {
        m_currentDevice = availableDevices.first();


        //设置语音格式
        //格式支持：pcm（不压缩）、wav（不压缩，pcm编码）、amr（压缩格式）。推荐pcm 采样率 ：16000 固定值。 编码：16bit 位深的单声道。
        QAudioFormat format;
        format.setSampleRate(8000);
        format.setChannelCount(1);
        format.setSampleSize(16);
        format.setSampleType(QAudioFormat::SignedInt);
        format.setByteOrder(QAudioFormat::LittleEndian);
        format.setCodec("audio/pcm");

        m_audioInput = new QAudioInput(m_currentDevice, format, this);
    }
}

void BaiduVop::setDevice(const QAudioDeviceInfo &device)
{
    m_currentDevice = device;
}



//使用Client Credentials获取Access Token需要应用在其服务端发送请求（推荐用POST方法）到百度OAuth2.0授权服务的“ https://openapi.baidu.com/oauth/2.0/token ”地址上，并带上以下参数：

//grant_type：必须参数，固定为“client_credentials”；
//client_id：必须参数，应用的 API Key；
//client_secret：必须参数，应用的 Secret Key;
bool BaiduVop::refreshToken(void)
{
    QNetworkRequest request(QUrl(QString("https://openapi.baidu.com/oauth/2.0/token?")));
    QByteArray append = QString("grant_type=client_credentials&client_id=%1&client_secret=%2&").arg("1NCcvMhMMafRhLvyNg6kePou").arg("G606YPDBNMWUIKe8cN9CLwITlmrXXjpl").toLatin1();
    QByteArray buf;

    request.setRawHeader("Content-Type", "application/json");

    const auto &&flag = m_http.post(request, append, buf, 15000);
    if(!flag) { return false; }

    const auto &&data = QJsonDocument::fromJson(buf).object();
    if(data.isEmpty() || !data.contains("access_token")) { return false; }

    m_token = data["access_token"].toString();
    return true;
}

bool BaiduVop::start(void)
{
    if(m_token.isEmpty())
    {
        qDebug("BaiduVop::start fail, Need refresh token befor start.");
        return false;
    }

    m_buffer = new QBuffer;
    m_buffer->open(QIODevice::ReadWrite);
    m_audioInput->start(m_buffer);

    return true;
}

std::pair<bool, QString> BaiduVop::finish(void)
{
    m_audioInput->stop();

    const auto &sendData = m_buffer->data();
    m_buffer->deleteLater();

    QNetworkRequest request(QUrl("http://vop.baidu.com/server_api"));
    QJsonObject append;

    request.setRawHeader("Content-Type", "application/json");

    append["format"] = "pcm";
    append["rate"] = 8000;
    append["channel"] = 1;
    append["token"] = m_token;
    append["lan"] = "zh";
    append["cuid"] = "JasonQt";
    append["speech"] = QString(sendData.toBase64());
    append["len"] = sendData.size();

    QByteArray buf;
    m_http.post(request, QJsonDocument(append).toJson(), buf, 15000);

    QJsonObject acceptedData(QJsonDocument::fromJson(buf).object());

    if(buf.isEmpty() || acceptedData.isEmpty() || !acceptedData.contains("result")) { return { false, buf }; }

    const auto &&message = acceptedData["result"].toArray()[0].toString();
    return { true, message.mid(0, message.size() - 1) };
}
