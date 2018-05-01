using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Media.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x804 上介绍了“空白页”项模板

namespace Week8
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    /// 

    class MusicConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, string language)
        {
            return ((TimeSpan)value).TotalSeconds;
        }

        public object ConvertBack(object value, Type targetType, object parameter, string language)
        {
            return TimeSpan.FromSeconds((double)value);
        }
    }

    public sealed partial class MainPage : Page
    {
        bool toggle;
        public MainPage()
        {
            this.InitializeComponent();
            toggle = true;//poster在前台为true，poster在后台为false
        }
        //a converter for mediaplayer to change double to TimeSpan.

        private void toggle_solve()
        {
            //toggle为false时是电影模式，切换时mediaPlayer要在前台.
            if(toggle == true)
            {
                mediaPlayer.Width = media_poster.Width;
                mediaPlayer.Height = media_poster.Height;
                mediaPlayer.Margin = media_poster.Margin;
                media_poster.Width = 0;
                media_poster.Height = 0;
                media_poster.Margin = new Thickness(0);
                toggle = false;
            }
            else//mediaPlayer在后台
            {
                myStoryboard.Stop();
                media_poster.Width = mediaPlayer.Width;
                media_poster.Height = mediaPlayer.Height;
                media_poster.Margin = mediaPlayer.Margin;
                mediaPlayer.Width = 0;
                mediaPlayer.Height = 0;
                mediaPlayer.Margin = new Thickness(0);
                toggle = true;
            }
        }

        private void media_MediaOpened(object sender, RoutedEventArgs e)
        {
            mediaPlayerSlider.Maximum = mediaPlayer.NaturalDuration.TimeSpan.TotalSeconds;
        }

        private void media_Play(object sender, RoutedEventArgs e)
        {
            myStoryboard.Begin();
            mediaPlayer.Play();
        }

        private void media_Pause(object sender, RoutedEventArgs e)
        {
            myStoryboard.Pause();
            mediaPlayer.Pause();
        }

        private void media_Stop(object sender, RoutedEventArgs e)
        {
            myStoryboard.Stop();
            mediaPlayer.Stop();
        }

        async private void media_Add(object sender, RoutedEventArgs e)
        {
            var openPicker = new Windows.Storage.Pickers.FileOpenPicker();

            openPicker.FileTypeFilter.Add(".wmv");
            openPicker.FileTypeFilter.Add(".mp4");
            openPicker.FileTypeFilter.Add(".wma");
            openPicker.FileTypeFilter.Add(".mp3");

            var file = await openPicker.PickSingleFileAsync();

            if(file!=null)
            {
                var stream = await file.OpenAsync(Windows.Storage.FileAccessMode.Read);
                mediaPlayer.SetSource(stream, file.ContentType);
                string type = file.ContentType;
                int indexOf = type.IndexOf("/");
                string originType = type.Substring(0, indexOf);
                if((originType.Equals("audio")&&toggle==false)||(originType.Equals("video") && toggle == true))
                {
                    toggle_solve();
                }
                mediaPlayer.Play();
            }
        }
        private void media_Video(object sender, RoutedEventArgs e)
        {
            if (toggle == true)
            {
                toggle_solve();
            }
            mediaPlayer.Source = new Uri("ms-appx:///Assets/Videos/1.mp4");
            mediaPlayer.Play();
        }

        private void media_FullScreen(object sender, RoutedEventArgs e)
        {
            mediaPlayer.IsFullWindow = !mediaPlayer.IsFullWindow;
        }
        private void ChangeMediaVolume(object sender, RoutedEventArgs e)
        {
            mediaPlayer.Volume = (double)volumeSlider.Value;
        }

        private void media_Volume(object sender, RoutedEventArgs e)
        {
            volumeControl.Opacity = (volumeControl.Opacity == 0) ? 1 : 0;
        }
    }
}
