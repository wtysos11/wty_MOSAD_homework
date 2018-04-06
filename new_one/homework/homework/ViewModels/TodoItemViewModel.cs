using System.Collections.ObjectModel;
using Windows.UI.Xaml.Media.Imaging;

namespace Todos.ViewModels
{
    class TodoItemViewModel
    {
        private ObservableCollection<Models.TodoItem> allItems = new ObservableCollection<Models.TodoItem>();
        public ObservableCollection<Models.TodoItem> AllItems { get { return this.allItems; } }

        private Models.TodoItem selectedItem;
        public Models.TodoItem SelectedItem { get { return selectedItem; } set { this.selectedItem = value; } }

        public TodoItemViewModel()
        {
            this.allItems.Add(new Models.TodoItem("test1", "just test1", "2018/4/29", null));
            this.allItems.Add(new Models.TodoItem("test2", "just test2", "2018/3/29", null));
        }

        public void AddTodoItem(string title, string description, string time, BitmapImage bitmap)
        {
            this.allItems.Add(new Models.TodoItem(title, description, time, bitmap));
        }

        public void RemoveTodoItem(Models.TodoItem item)
        {
            this.allItems.Remove(item);
            this.selectedItem = null;
        }

        public void UpdateTodoItem(string time, string title, string description)
        {
            this.selectedItem.title = title;
            this.selectedItem.description = description;
            this.SelectedItem.SetTime(time);
            this.selectedItem = null;
        }

        public void ChangeURI(BitmapImage otherBitmap)
        {
            this.selectedItem.bitmap = otherBitmap;
        }
    }
}
