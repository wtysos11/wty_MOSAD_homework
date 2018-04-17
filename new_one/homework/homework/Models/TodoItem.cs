using System;
using System.Globalization;
using System.ComponentModel;
using Windows.UI.Xaml.Media.Imaging;
using System.Text;

namespace Todos.Models
{
    class TodoItem
    {
        private string id;
        private bool isCompleted;

        public string GetId() { return id; }
        public void setID(string _id)
        {
            this.id = _id;
        }
        public string title { get; set; }
        public string description { get; set; }
        public DateTime time { get; set; }
        public BitmapImage bitmap { get; set; }

        public bool completed
        {
            get { return isCompleted; }
            set { isCompleted = value; }
        }

        public int completedLine
        {
            get { if (isCompleted == true) return 1; else return 0; }
        }

        public void setComplete(int i)
        {
            if (i == 1)
                isCompleted = true;
            else
                isCompleted = false;
        }

        public void SetTime(string time)
        {
            DateTimeFormatInfo dateFormat = new DateTimeFormatInfo();
            dateFormat.ShortDatePattern = "yyyy/MM/dd";
            DateTime nowTime = Convert.ToDateTime(time, dateFormat);
            this.time = nowTime;
        }

        public TodoItem(string title, string description, string time, BitmapImage bitmap)
        {
            this.id = Guid.NewGuid().ToString(); //生成id
            this.title = title;
            this.description = description;
            if (bitmap == null)
            {
                this.bitmap = new BitmapImage(new Uri("ms-appx:///Assets/star.jpg"));
            }
            else
            {
                this.bitmap = bitmap;
            }
            SetTime(time);
            this.isCompleted = false; //默认为未完成
        }
        public TodoItem(string id,string title, string description, string time, BitmapImage bitmap)
        {
            this.id = id; //生成id
            this.title = title;
            this.description = description;
            if (bitmap == null)
            {
                this.bitmap = new BitmapImage(new Uri("ms-appx:///Assets/star.jpg"));
            }
            else
            {
                this.bitmap = bitmap;
            }
            SetTime(time);
            this.isCompleted = false; //默认为未完成
        }
        public string getURI()
        {
            return bitmap.UriSource.ToString();
        }
        public StringBuilder getStringBuilder()
        {
            StringBuilder builder = new StringBuilder();
            builder.AppendFormat("Title:{0} Description:{1} Time:{2}", this.title, this.description, this.time.ToString());
            return builder;
        }
    }
}
