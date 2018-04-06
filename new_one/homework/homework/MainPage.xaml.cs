using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage;
using Windows.Storage.Pickers;
using Windows.Storage.Streams;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Media.Imaging;
using Windows.UI.Xaml.Navigation;



namespace Todos
{
    public sealed partial class MainPage : Page
    {

        public MainPage()
        {
            this.InitializeComponent();
            this.ViewModel = new ViewModels.TodoItemViewModel();
            bitmapCache = new BitmapImage(new Uri("ms-appx:///Assets/star.jpg"));
        }

        ViewModels.TodoItemViewModel ViewModel { get; set; }
        public BitmapImage bitmapCache;

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            if (e.Parameter.GetType() == typeof(ViewModels.TodoItemViewModel))
            {
                this.ViewModel = (ViewModels.TodoItemViewModel)(e.Parameter);
            }

            if(e.NavigationMode == NavigationMode.New)
            {
                ApplicationData.Current.LocalSettings.Values.Remove("newpage");
            }
            else
            {
                if(ApplicationData.Current.LocalSettings.Values.ContainsKey("newpage"))
                {
                    var composite = ApplicationData.Current.LocalSettings.Values["newpage"] as ApplicationDataCompositeValue;
                    title_MainPage.Text = (string)composite["title"];
                    description_MainPage.Text = (string)composite["description"];
                    DatePicker_MainPage.Date = (DateTimeOffset)composite["date"];
                    image_MainPage.Source = new BitmapImage(new Uri("ms-appx://homework/"+(string)composite["image_uri"]));
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
                composite["title"] = title_MainPage.Text;
                composite["description"] = description_MainPage.Text;
                composite["date"] = DatePicker_MainPage.Date;
                composite["image_uri"] = image_MainPage.Source;
                ApplicationData.Current.LocalSettings.Values["newpage"] = composite;
            }
        }

        private void TodoItem_ItemClicked(object sender, ItemClickEventArgs e)
        {
            ViewModel.SelectedItem = (Models.TodoItem)(e.ClickedItem);
            if (AnotherGrid.Visibility == Visibility.Collapsed)//宽度小于800
                Frame.Navigate(typeof(NewPage), ViewModel);
            else
            {
                CreateButton.Content = "Update";
                title_MainPage.Text = ViewModel.SelectedItem.title;
                description_MainPage.Text = ViewModel.SelectedItem.description;
                DatePicker_MainPage.Date = ViewModel.SelectedItem.time;
                image_MainPage.Source = ViewModel.SelectedItem.bitmap;
            }
        }

        private void AddAppBarButton_Click(object sender, RoutedEventArgs e)//创建
        {
            ViewModel.SelectedItem = null;
            if (AnotherGrid.Visibility == Visibility.Collapsed)
                Frame.Navigate(typeof(NewPage), ViewModel);
        }

        private void Delete_Click(object sender, RoutedEventArgs e)
        {
            var datacontext = (sender as FrameworkElement).DataContext;
            var item = ToDoListView.ContainerFromItem(datacontext) as ListViewItem;
            ViewModel.SelectedItem = (Models.TodoItem)(item.Content);
            ViewModel.RemoveTodoItem(ViewModel.SelectedItem);
        }

        private void Edit_Click(object sender, RoutedEventArgs e)//编辑
        {
            var datacontext = (sender as FrameworkElement).DataContext;
            var item = ToDoListView.ContainerFromItem(datacontext) as ListViewItem;
            ViewModel.SelectedItem = (Models.TodoItem)(item.Content);
            Frame.Navigate(typeof(NewPage), ViewModel);
        }


        private void Main_Cancel(object sender, RoutedEventArgs e)
        {
            title_MainPage.Text = String.Empty;
            description_MainPage.Text = String.Empty;
            DatePicker_MainPage.Date = DateTimeOffset.Now;
            CreateButton.Content = "Create";
        }

        private void CheckBox_check(object sender, RoutedEventArgs e)
        {
            Frame.Navigate(typeof(MainPage), ViewModel);
        }

        private void UpdateButton_Clicked(object sender, RoutedEventArgs e)
        {
            if (ViewModel.SelectedItem != null)
            {
                ViewModel.UpdateTodoItem(DatePicker_MainPage.Date.ToString(), title_MainPage.Text, description_MainPage.Text);
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

        private void Main_Create(object sender, RoutedEventArgs e)
        {
            bool TimeState = true;
            bool titleEmpty = false;
            bool descriptionEmpty = false;

            if (DatePicker_MainPage.Date.AddDays(1) < DateTimeOffset.Now) TimeState = false;
            if (title_MainPage.Text.Trim() == String.Empty) titleEmpty = true;
            if (description_MainPage.Text.Trim() == String.Empty) descriptionEmpty = true;

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

            if (CreateButton.Content.ToString() != "Update")
            {
                if (!titleEmpty && !descriptionEmpty && TimeState)
                {
                    string timeStr = DatePicker_MainPage.Date.ToString();
                    ViewModel.AddTodoItem(title_MainPage.Text, description_MainPage.Text, timeStr, bitmapCache);
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
            Frame.Navigate(typeof(MainPage), ViewModel);
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
                image_MainPage.Source = bitmap;
            }
        }

    }
}
