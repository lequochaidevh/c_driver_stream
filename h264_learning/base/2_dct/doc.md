đọc file PGM bằng fread() thẳng vào buffer, mà file PGM có header ASCII (P5\nW H\n255\n) trước phần dữ liệu nhị phân. Kết quả: mảng Y[] nhận cả header -> toàn bộ các block DCT bị xô lệch → dct_blocks.csv và heatmap sai.


