using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using System.Net;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Text;
using System.Text.RegularExpressions;
using Windows.Data.Xml.Dom;
// https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x804 上介绍了“空白页”项模板

namespace Week7
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        private void TextBlock_SelectionChanged(object sender, RoutedEventArgs e)
        {
            getNetworkCellphone(Telphone_text.Text);
        }

        private void Weather_button_Click(object sender, RoutedEventArgs e)
        {
            getNetworkWeather(Weather_text.Text);
        }

        //根据位置返回请求URL
        public string getWeatherURL(string location)
        {
            //api具体信息https://www.seniverse.com/doc#language
            return "https://api.seniverse.com/v3/weather/now.json?key=yx5mzkupk8rrp88l&location=" +location+"&language=zh-Hans&unit=c";
        }

        public string getCellphoneURL(string number)
        {
            //api具体信息https://www.nowapi.com/?app=intf.appkey
            return "http://api.k780.com/?app=idcard.get&idcard=" + number.Trim() + "&appkey=33131&sign=d6470330f3812ebeb06483b1336fdd77&format=xml";
            //return "http://api.k780.com/?app=phone.get&phone=" + number.Trim() + "&appkey=33131&sign=d6470330f3812ebeb06483b1336fdd77&format=xml";
        }

        //使用get方法来请求具体位置Location的天气信息
        public async void getNetworkWeather(string location)
        {
            try
            {
                HttpWebRequest request = WebRequest.Create(getWeatherURL(location)) as HttpWebRequest;
                HttpWebResponse response = await request.GetResponseAsync() as HttpWebResponse;
                System.IO.Stream readResponseStream = response.GetResponseStream();
                using (System.IO.StreamReader reader = new System.IO.StreamReader(readResponseStream))
                {
                    string jsonText=reader.ReadToEnd();
                    JObject jo = (JObject)JsonConvert.DeserializeObject(jsonText);

                    Weather_result.Text = convertJSON2String(jo);
                }
            }
            catch(System.Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.Message);
                Weather_result.Text = ex.Message;
            }


        }
        private string convertJSON2String(JObject jo)
        {
            StringBuilder sb = new StringBuilder("当前天气\n" + "");
            var result = jo["results"][0];
            sb.Append("位置：" + result["location"]["name"] + "\n");
            sb.Append("具体位置：" + result["location"]["path"] + "\n");
            sb.Append("当前天气：" + result["now"]["text"] + "\n");
            sb.Append("当前温度：" + result["now"]["temperature"] + "\n");
            return sb.ToString();
        }

        public async void getNetworkCellphone(string number)
        {
            try
            {
                HttpWebRequest request = WebRequest.Create(getCellphoneURL(number)) as HttpWebRequest;
                HttpWebResponse response = await request.GetResponseAsync() as HttpWebResponse;
                System.IO.Stream readResponseStream = response.GetResponseStream();
                using (System.IO.StreamReader reader = new System.IO.StreamReader(readResponseStream))
                {
                    StringBuilder returnString=new StringBuilder();
                    string xmlText = reader.ReadToEnd();
                    XmlDocument doc = new XmlDocument();
                    doc.LoadXml(xmlText);

                    XmlNodeList listNodes = null;
                    listNodes = doc.DocumentElement.ChildNodes;

                    //拿到第二个标签（result）的元素
                    XmlElement result = listNodes.Item(1) as XmlElement;
                    XmlNodeList list = null;
                    list = result.ChildNodes;
                    foreach(XmlElement node in list)
                    {
                        returnString.Append(node.TagName + ": " + node.InnerText + "\n");
                    }

                    Telphone_result.Text = returnString.ToString();
                }
            }
            catch (System.Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.Message);
                Telphone_result.Text = ex.Message;
            }
        }
    }
}
