using System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;
using Windows.UI.Popups;
using Windows.Storage.Pickers;
using Windows.UI.Xaml.Media.Imaging;
using Windows.Storage;
using Windows.Storage.Streams;

namespace Todos
{
    public sealed partial class NewPage : Page
    {
        public NewPage()
        {
            this.InitializeComponent();
            ViewModel = ViewModels.TodoItemViewModel.getInstance();
        }

        private ViewModels.TodoItemViewModel ViewModel;
        public BitmapImage bitmapCache;

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            bitmapCache = new BitmapImage(new Uri("ms-appx:///Assets/star.jpg"));

            if (ViewModel.SelectedItem == null)
            {
                CreateButton.Content = "Create";
                var button = new MessageDialog("请创建Todo项目！").ShowAsync();
            }
            else
            {
                CreateButton.Content = "Update";
                var button = new MessageDialog("请修改Todo项目！").ShowAsync();

                title.Text = ViewModel.SelectedItem.title;
                description.Text = ViewModel.SelectedItem.description;
                DatePicker.Date = ViewModel.SelectedItem.time;
                StarPic.Source = ViewModel.SelectedItem.bitmap;
                // testblock.Text = StarPic.Source.ToString();//debug
            }

            if (e.NavigationMode == NavigationMode.New)
            {
                ApplicationData.Current.LocalSettings.Values.Remove("newpage");
            }
            else
            {
                if (ApplicationData.Current.LocalSettings.Values.ContainsKey("newpage"))
                {
                    var composite = ApplicationData.Current.LocalSettings.Values["newpage"] as ApplicationDataCompositeValue;
                    title.Text = (string)composite["title"];
                    description.Text = (string)composite["description"];
                    DatePicker.Date = (DateTimeOffset)composite["date"];
                    StarPic.Source = new BitmapImage(new Uri("ms-appx://homework/" + (string)composite["image_uri"]));
                }
            }
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            bool suspending = ((App)App.Current).isSuspend;
            if (suspending)
            {
                ApplicationDataCompositeValue composite = new ApplicationDataCompositeValue();
                //保存多项配置
                composite["title"] = title.Text;
                composite["description"] = description.Text;
                composite["date"] = DatePicker.Date;
                composite["image_uri"] = StarPic.Source;
                ApplicationData.Current.LocalSettings.Values["newpage"] = composite;
            }
        }

        private async void checkOut(int stateNum)
        {
            if (stateNum == 1)
            {
                ContentDialog warningDialog = new ContentDialog()
                {
                    Title = "Title和Details的内容为空",
                    Content = "请先输入Title和Details的内容",
                    PrimaryButtonText = "Ok"
                };
                ContentDialogResult result = await warningDialog.ShowAsync();
            }
            if (stateNum == 2)
            {
                ContentDialog warningDialog = new ContentDialog()
                {
                    Title = "Title的内容为空",
                    Content = "请先输入Title的内容",
                    PrimaryButtonText = "Ok"
                };
                ContentDialogResult result = await warningDialog.ShowAsync();
            }
            if (stateNum == 3)
            {
                ContentDialog warningDialog = new ContentDialog()
                {
                    Title = "Details的内容为空",
                    Content = "请先输入Details的内容",
                    PrimaryButtonText = "Ok"
                };
                ContentDialogResult result = await warningDialog.ShowAsync();
            }
            if (stateNum == 4)
            {
                ContentDialog warningDialog = new ContentDialog()
                {
                    Title = "Title和Details的内容为空,且日期不合法（大于等于今天）",
                    Content = "请先输入Title和Details的内容并且修改日期",
                    PrimaryButtonText = "Ok"
                };
                ContentDialogResult result = await warningDialog.ShowAsync();
            }
            if (stateNum == 5)
            {
                ContentDialog warningDialog = new ContentDialog()
                {
                    Title = "Title的内容为空，且日期不合法（需要大于等于今天）",
                    Content = "请先输入Title的内容并且修改日期",
                    PrimaryButtonText = "Ok"
                };
                ContentDialogResult result = await warningDialog.ShowAsync();
            }
            if (stateNum == 6)
            {
                ContentDialog warningDialog = new ContentDialog()
                {
                    Title = "Details的内容为空，且日期不合法（需要大于等于今天）",
                    Content = "请先输入Details的内容并且修改日期",
                    PrimaryButtonText = "Ok"
                };
                ContentDialogResult result = await warningDialog.ShowAsync();
            }
            if (stateNum == 7)
            {
                ContentDialog warningDialog = new ContentDialog()
                {
                    Title = "日期不合法（需要大于等于今天）",
                    Content = "请修改日期",
                    PrimaryButtonText = "Ok"
                };
                ContentDialogResult result = await warningDialog.ShowAsync();
            }
        }

        private void DeleteButton_Clicked(object sender, RoutedEventArgs e)
        {
            if (ViewModel.SelectedItem != null)
            {
                ViewModel.RemoveTodoItem(ViewModel.SelectedItem);
                Frame.Navigate(typeof(MainPage));
            }
        }
        private void CreateButton_Clicked(object sender, RoutedEventArgs e)
        {
            //judge whether qualification satisfied
            bool TimeState = true;
            bool titleEmpty = false;
            bool descriptionEmpty = false;

            if (DatePicker.Date.AddDays(1) < DateTimeOffset.Now) TimeState = false;
            if (title.Text.Trim() == String.Empty) titleEmpty = true;
            if (description.Text.Trim() == String.Empty) descriptionEmpty = true;

            if (titleEmpty && descriptionEmpty && TimeState)
                checkOut(1);
            if (titleEmpty && !descriptionEmpty && TimeState)
                checkOut(2);
            if (!titleEmpty && descriptionEmpty && TimeState)
                checkOut(3);
            if (titleEmpty && descriptionEmpty && !TimeState)
                checkOut(4);
            if (titleEmpty && !descriptionEmpty && !TimeState)
                checkOut(5);
            if (!titleEmpty && descriptionEmpty && !TimeState)
                checkOut(6);
            if (!titleEmpty && !descriptionEmpty && !TimeState)
                checkOut(7);


            if (ViewModel.SelectedItem == null)
            {
                if (!titleEmpty && !descriptionEmpty && TimeState)
                {
                    string timeStr = DatePicker.Date.ToString();
                    ViewModel.AddTodoItem(title.Text, description.Text, timeStr, bitmapCache);
                    Frame.Navigate(typeof(MainPage));
                }
            }
            else
            {
                if (!titleEmpty && !descriptionEmpty && TimeState)
                {
                    ViewModel.ChangeURI(bitmapCache);
                    UpdateButton_Clicked(sender, e);
                }
            }
        }

        private void UpdateButton_Clicked(object sender, RoutedEventArgs e)
        {
            if (ViewModel.SelectedItem != null)
            {
                ViewModel.UpdateTodoItem(DatePicker.Date.ToString(), title.Text, description.Text);
                Frame.Navigate(typeof(MainPage));
            }
        }

        private void CancelButton_Clicked(object sender, RoutedEventArgs e)
        {
            title.Text = String.Empty;
            description.Text = String.Empty;
            DatePicker.Date = DateTimeOffset.Now;
        }

        private async void SelectButton_Clicked(object sender, RoutedEventArgs e)
        {
            FileOpenPicker picker = new FileOpenPicker();
            // Initialize the picture file type to take
            picker.FileTypeFilter.Add(".jpg");
            picker.FileTypeFilter.Add(".jpeg");
            picker.FileTypeFilter.Add(".png");
            picker.FileTypeFilter.Add(".bmp");
            picker.SuggestedStartLocation = PickerLocationId.PicturesLibrary;

            StorageFile file = await picker.PickSingleFileAsync();

            if (file != null)
            {
                // Load the selected picture
                IRandomAccessStream stream = await file.OpenAsync(FileAccessMode.Read);
                BitmapImage bitmap = new BitmapImage();
                await bitmap.SetSourceAsync(stream);
                bitmapCache = bitmap;
                StarPic.Source = bitmap;
            }
        }
    }
}
