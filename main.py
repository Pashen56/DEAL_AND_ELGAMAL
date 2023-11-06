import tkinter as tk
from tkinter import filedialog
from tkinter import ttk
from tkinter import messagebox
from tkinter import simpledialog
import time
import threading
from ctypes import cdll


# Загрузка библиотек
mylib_DEAL = cdll.LoadLibrary("./DEAL.dylib")
mylib_ELGAMAL = cdll.LoadLibrary("./ELGAMAL.dylib")

def reset_fields():
    file_entry.delete(0, tk.END)
    method_choice.set("")
    output_entry.delete(0, tk.END)
# Функция для симуляции операции шифрования/дешифрования с обновлением прогресса
def process_file():
    selected_file_path = file_entry.get()
    selected_method = method_choice.get()
    output_directory = output_entry.get()
    print(selected_file_path)
    print(selected_method)
    print(output_directory)

    file = selected_file_path.encode('cp866')
    directorytosave = output_directory.encode('cp866')

    if (selected_method == "DEAL"):
        mylib_DEAL.Encryption(file, directorytosave, 256)
        print("DEAL")
    else:
        mylib_ELGAMAL.Encryption(file, directorytosave, 400)
        print("ELGAMAL")


    if not selected_file_path or not output_directory:
        messagebox.showerror("Ошибка", "Выберите файл и директорию для сохранения.")
        return

    # Симулируем операцию
    max_progress = 100
    for i in range(max_progress + 1):
        time.sleep(0.05)  # Симуляция длительной операции
        progress.set(i)  # Обновляем значение прогрессбара

    # Здесь вы можете добавить код для реальной операции шифрования/дешифрования.
    messagebox.showinfo("Завершено", "Действие выполнено успешно!")
    f = threading.Thread(target=reset_fields)
    f.start()

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

output_label = ttk.Label(frame1, text="Выберите директорию для сохранения файла:")
output_label.grid(row=2, column=0)

output_entry = ttk.Entry(frame1, width=50)
output_entry.grid(row=2, column=1)

browse_output_button = ttk.Button(frame1, text="Обзор", command=lambda: output_entry.insert(0, filedialog.askdirectory(title="Выберите директорию для сохранения файла")))
browse_output_button.grid(row=2, column=2)

progress = tk.DoubleVar()
progress.set(0)
progress_bar = ttk.Progressbar(frame1, variable=progress, maximum=100)
progress_bar.grid(row=3, columnspan=3)

next_button = ttk.Button(frame1, text="Далее", command=start_processing)
next_button.grid(row=4, columnspan=3)

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











"""
from ctypes import cdll

# Загрузка библиотек
mylib_DEAL = cdll.LoadLibrary("./DEAL_1.dylib")
mylib_ELGAMAL = cdll.LoadLibrary("./ELGAMAL_1.dylib")

# Работа с файлами
file_file_txt = "/Users/muraddaudov/PycharmProjects/DEAL_AND_ELGAMAL/file.txt"
f_file_file_txt = file_file_txt.encode('cp866')

file_text_txt = "/Users/muraddaudov/PycharmProjects/DEAL_AND_ELGAMAL/text.txt"
f_file_text_txt = file_text_txt.encode('cp866')

file_img = "/Users/muraddaudov/PycharmProjects/DEAL_AND_ELGAMAL/img.jpg"
f_file_img = file_img.encode('cp866')

file_sound = "/Users/muraddaudov/PycharmProjects/DEAL_AND_ELGAMAL/sound.wav"
f_file_sound = file_sound.encode('cp866')

file_video = "/Users/muraddaudov/PycharmProjects/DEAL_AND_ELGAMAL/video.mp4"
f_file_video = file_video.encode('cp866')

# Вызов функций
mylib_DEAL.Encryption(f_file_file_txt, 192)
mylib_DEAL.Encryption(f_file_text_txt, 128)
mylib_DEAL.Encryption(f_file_img, 128)
mylib_DEAL.Encryption(f_file_sound, 256)
mylib_DEAL.Encryption(f_file_video, 128)

mylib_ELGAMAL.ElgamalEncryption(f_file_file_txt, 400)
"""