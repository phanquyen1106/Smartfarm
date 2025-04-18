# 🔌 Smartfarm

> Mô tả ngắn gọn: 
- Dự án dùng để tưới tiêu tự động
- Nông dân dùng chính
- Mục tiêu thiết kế là để ra sản phẩm tưới tiêu tự động

## 📑 Mục Lục

- [Giới thiệu](#giới-thiệu)
- [Thông số kỹ thuật](#thông-số-kỹ-thuật)
- [Danh sách linh kiện](#danh-sách-linh-kiện)
- [Sơ đồ nguyên lý và PCB](#sơ-đồ-nguyên-lý-và-pcb)
- [Hướng dẫn lắp ráp](#hướng-dẫn-lắp-ráp)
- [Lập trình firmware](#lập-trình-firmware)
- [Cách sử dụng](#cách-sử-dụng)
- [Kiểm thử](#kiểm-thử)
- [Ảnh/Video demo](#ảnhvideo-demo)
- [Đóng góp](#đóng-góp)
- [Giấy phép](#giấy-phép)

---

## 👋 Giới Thiệu

Trình bày ngắn gọn:
- Dự án dùng để tưới tiêu tự động
- Người nông dân dùng 
- Mục tiêu thiết kế là để ra sản phẩm tưới tiêu tự động

---

## 📐 Thông Số Kỹ Thuật

| Thành phần     | Thông tin            |
|----------------|----------------------|
| MCU            | ESP32-WROOM-32       |
| Nguồn vào      | 5V qua USB hoặc DC   |
| Kết nối        | WiFi, Bluetooth      |
| Kích thước PCB | 80mm x 99mm          |

---

## 🧰 Danh Sách Linh Kiện

| Tên linh kiện            | Số lượng | Ghi chú                     |
|--------------------------|----------|-----------------------------|
| ESP32 DevKit v1          | 1        | Vi điều khiển chính         |
| Module cảm biến đất      | 1        | Vòng tròn hoặc dải LED      |
| DHT11                    | 1        | Cảm biến nhiệt độ, độ ẩm    |
| Led 3 cái                | 3        | Led thường                  |
| Mạch hã áp 2596          | 1        | Điều khiển thủ công         |

*Có thể link đến file BOM.csv đầy đủ.*

---

## 🔧 Sơ Đồ Nguyên Lý và PCB

- 📎 [Schematic (PDF)](docs/schematic.pdf)
- 📎 [PCB Layout (Gerber)](docs/gerber.zip)
- 📎 [File thiết kế (Eagle / KiCad)](docs/project.kicad_pcb)

_Hình minh họa sơ đồ nguyên lý hoặc board PCB có thể nhúng ngay tại đây:_

![Schematic](docs/images/schematic.png)

---

## 🔩 Hướng Dẫn Lắp Ráp

1. Hàn các linh kiện nhỏ trước: điện trở, tụ điện
2. Hàn vi điều khiển hoặc socket
3. Kiểm tra ngắn mạch bằng đồng hồ
4. Cấp nguồn thử, kiểm tra dòng tiêu thụ


## 💻 Lập Trình Firmware

- Lập Trình Hệ Thống Giám Sát
- Ngôn ngữ & nền tảng: JavaScript (Node.js) / HTML-CSS-JS / MQTT / Chart.js
- Giao tiếp: Sử dụng giao thức MQTT để trao đổi dữ liệu giữa cảm biến và máy chủ.
- Lưu trữ và hiển thị dữ liệu:
- Giao diện Web hiển thị: nhiệt độ, độ ẩm, ánh sáng, tốc độ gió, độ ẩm đất...
- Biểu đồ thời gian thực được cập nhật qua Chart.js.
- Thư viện và công cụ sử dụng:
- mqtt, express, socket.io, chart.js, font-awesome...
