using System.Collections.ObjectModel;
using Windows.UI.Xaml.Media.Imaging;
using SQLitePCL;
using System.Globalization;
using System;

namespace Todos.ViewModels
{
    class TodoItemViewModel
    {
        private SQLiteConnection conn;
        private ObservableCollection<Models.TodoItem> allItems = new ObservableCollection<Models.TodoItem>();
        public ObservableCollection<Models.TodoItem> AllItems { get { return this.allItems; } }

        private Models.TodoItem selectedItem;
        public Models.TodoItem SelectedItem { get { return selectedItem; } set { this.selectedItem = value; } }

        public bool first = false;
        private static TodoItemViewModel _instance;
        public static TodoItemViewModel getInstance()
        {
            if(_instance == null)
            {
                _instance = new TodoItemViewModel();
            }

            return _instance;
        }

        public TodoItemViewModel()
        {
            conn = new SQLiteConnection("demo.db");
            using (var statement = conn.Prepare("CREATE TABLE IF NOT EXISTS todolist (id CHAR(36),title VARCHAR(255),description VARCHAR(255),deadline DATE, PRIMARY KEY (id));"))
            {
                statement.Step();
            }
            using (var statement = conn.Prepare("SELECT * FROM todolist;"))
            {
                while (statement.Step() == SQLiteResult.ROW)
                {
                    Models.TodoItem item = new Models.TodoItem((string)statement[0], (string)statement[1], (string)statement[2], (string)statement[3], null);
                    this.allItems.Add(item);
                }
            }
        }

        public void AddTodoItem(string title, string description, string time, BitmapImage bitmap)
        {
            
            this.allItems.Add(new Models.TodoItem(title, description, time, bitmap));
            Models.TodoItem item = allItems[allItems.Count - 1];
            using (var statement = conn.Prepare("INSERT INTO todolist VALUES(?,?,?,?)"))
            {
                statement.Bind(1, item.GetId());
                statement.Bind(2, item.title);
                statement.Bind(3, item.description);

                DateTimeFormatInfo dateFormat = new DateTimeFormatInfo();
                dateFormat.ShortDatePattern = "yyyy/MM/dd";
                DateTime nowTime = Convert.ToDateTime(item.time, dateFormat);
                statement.Bind(4, nowTime.Year.ToString()+"-"+nowTime.Month.ToString()+"-"+nowTime.Day.ToString());
                statement.Step();
            }
        }

        public void RemoveTodoItem(Models.TodoItem item)
        {
            using (var statement = conn.Prepare("DELETE FROM todolist WHERE id = ?;"))
            {
                statement.Bind(1, item.GetId());
                statement.Step();
            }
            this.allItems.Remove(item);
            this.selectedItem = null;
        }

        public void UpdateTodoItem(string time, string title, string description)
        {
            this.selectedItem.title = title;
            this.selectedItem.description = description;
            this.selectedItem.SetTime(time);
            using (var statement = conn.Prepare("UPDATE todolist SET title = ?,description = ?,deadline = ? WHERE id = ?;"))
            {
                statement.Bind(1, title);
                statement.Bind(2, description);

                DateTimeFormatInfo dateFormat = new DateTimeFormatInfo();
                dateFormat.ShortDatePattern = "yyyy/MM/dd";
                DateTime nowTime = Convert.ToDateTime(this.selectedItem.time, dateFormat);
                statement.Bind(3, nowTime.Year.ToString() + "-" + nowTime.Month.ToString() + "-" + nowTime.Day.ToString());

                statement.Bind(4, this.selectedItem.GetId());
                statement.Step();
            }

            this.selectedItem = null;
        }

        public void ChangeURI(BitmapImage otherBitmap)
        {
            this.selectedItem.bitmap = otherBitmap;
        }
        public SQLiteConnection getSQLDB()
        {
            return conn;
        }
    }
}
