import tkinter as tk
from tkinter import filedialog
from tkinter import ttk
from tkinter import messagebox
from tkinter import Checkbutton
from tkinter import simpledialog
import time
import threading
from ctypes import cdll
import os

# Загрузка библиотек
mylib_DEAL = cdll.LoadLibrary("./DEAL.dylib")
mylib_ELGAMAL = cdll.LoadLibrary("./ELGAMAL.dylib")

mylib_DEAL.Keys(256)
mylib_ELGAMAL.Keys(400)

def reset_fields():
    file_entry.delete(0, tk.END)
    method_choice.set("")
    mode_choice.set("")
    output_entry.delete(0, tk.END)
# Функция для симуляции операции шифрования/дешифрования с обновлением прогресса
def process_file():
    selected_file_path = file_entry.get()
    selected_method = method_choice.get()
    selected_mode = mode_choice.get()
    output_directory = output_entry.get()
    print(selected_file_path)
    print(selected_method)
    print(selected_mode)
    print(output_directory)

    mode = 2

    if (selected_mode == "Шифрование"):
        mode = 1
    if (selected_mode == "Дешифрование"):
        mode = 0

    file = selected_file_path.encode('cp866')
    directorytosave = output_directory.encode('cp866')

    if (selected_method == "DEAL"):
        mylib_DEAL.Encryption(file, directorytosave, 256, mode)
        print("DEAL")
        print(mode)
    else:
        mylib_ELGAMAL.Encryption(file, directorytosave, 400, mode)
        print("ELGAMAL")
        print(mode)


    if not selected_file_path or not output_directory:
        messagebox.showerror("Ошибка", "Выберите файл и директорию для сохранения.")
        return

    if delete_file_var.get() and os.path.exists(selected_file_path):
        os.remove(selected_file_path)  # Удаляем файл, если галочка установлена

    # Симулируем операцию
    max_progress = 100
    for i in range(max_progress + 1):
        time.sleep(0.05)  # Симуляция длительной операции
        progress.set(i)  # Обновляем значение прогрессбара

    messagebox.showinfo("Завершено", "Действие выполнено успешно!")
    f = threading.Thread(target=reset_fields)
    f.start()
    print("=================================")

# Функция для выполнения операции в фоновом потоке
def start_processing():
    t = threading.Thread(target=process_file)
    t.start()



# Создаем окно
app = tk.Tk()
app.title("Шифрование и Дешифрование")

# Создаем первую страницу
frame1 = ttk.Frame(app)
frame1.pack()

# Переменная для отслеживания состояния галочки
delete_file_var = tk.BooleanVar()
delete_file_var.set(False)  # Устанавливаем значение по умолчанию

# Создаем виджет Checkbutton
delete_file_checkbutton = ttk.Checkbutton(frame1, text="Удалить первичный файл", variable=delete_file_var)
delete_file_checkbutton.grid(row=5, column=0, columnspan=3)

file_label = ttk.Label(frame1, text="Выберите файл:")
file_label.grid(row=0, column=0)

file_entry = ttk.Entry(frame1, width=50)
file_entry.grid(row=0, column=1)

browse_file_button = ttk.Button(frame1, text="Обзор", command=lambda: file_entry.insert(0, filedialog.askopenfilename()))
browse_file_button.grid(row=0, column=2)

method_label = ttk.Label(frame1, text="Выберите метод:")
method_label.grid(row=1, column=0)

method_choice = ttk.Combobox(frame1, values=["DEAL", "ELGAMAL"])
method_choice.grid(row=1, column=1)

mode_label = ttk.Label(frame1, text="Выберите режим:")
mode_label.grid(row=2, column=0)

mode_choice = ttk.Combobox(frame1, values=["Шифрование", "Дешифрование"])
mode_choice.grid(row=2, column=1)

output_label = ttk.Label(frame1, text="Выберите директорию для сохранения файла:")
output_label.grid(row=3, column=0)

output_entry = ttk.Entry(frame1, width=50)
output_entry.grid(row=3, column=1)

browse_output_button = ttk.Button(frame1, text="Обзор", command=lambda: output_entry.insert(0, filedialog.askdirectory(title="Выберите директорию для сохранения файла")))
browse_output_button.grid(row=3, column=2)

progress = tk.DoubleVar()
progress.set(0)
progress_bar = ttk.Progressbar(frame1, variable=progress, maximum=100)
progress_bar.grid(row=7, columnspan=3)

next_button = ttk.Button(frame1, text="Далее", command=start_processing)
next_button.grid(row=6, columnspan=3)

# Создаем вторую страницу
frame2 = ttk.Frame(app)
frame2.pack()

progress_label = ttk.Label(frame2, text="Прогресс:")
progress_label.grid(row=0, column=0)

progress_bar = ttk.Progressbar(frame2, mode="indeterminate")
progress_bar.grid(row=0, column=1)

back_button = ttk.Button(frame2, text="Назад", command=lambda: frame2.pack_forget())
back_button.grid(row=1, column=0, columnspan=2)

# Скрываем вторую страницу
frame2.pack_forget()

# Запускаем оконное приложение
app.mainloop()
