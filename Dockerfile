
### Dockerfile

# Используем официальный образ C++ с поддержкой C++17
FROM gcc:12.2.0

# Установка зависимостей
RUN apt-get update && \
    apt-get install -y \
    cmake \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Копируем исходный код
WORKDIR /app
COPY . .

# Создаем директорию для сборки
RUN mkdir build
WORKDIR /app/build

# Собираем проект
RUN cmake .. && make -j$(nproc)
CMD ["./examples/example2"]