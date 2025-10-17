1️⃣ Tổng quan
Hiểu cách JPEG dùng Huffman coding để nén dãy RLE (run-length + amplitude) thành bitstream hiệu quả nhất.
Sau RLE, ta có các cặp (run, value) biểu diễn số lượng zero liên tiếp và giá trị khác 0.
Ví dụ:

(0,5), (0,-3), (4,2), (0,-1), (0,0)


JPEG không lưu trực tiếp mấy cặp này mà mã hóa bằng Huffman, để:

Biểu diễn các giá trị thường xuất hiện (như nhiều số 0) bằng bit ngắn hơn

Giá trị hiếm hơn thì bit dài hơn




🔹 Bước 1: Gom dữ liệu từ RLE

Ví dụ, ta có mảng RLE sau khi zigzag:

int rle[][2] = {
    {0, 5}, {0, -3}, {4, 2}, {0, -1}, {0, 0}
};

🔹 Bước 2: Tính tần suất xuất hiện

Đếm xem mỗi giá trị (hoặc mỗi cặp) xuất hiện bao nhiêu lần.

🔹 Bước 3: Xây cây Huffman

Gộp 2 nút có tần suất thấp nhất lại.

Gán 0 cho nhánh trái, 1 cho nhánh phải.

Lặp lại cho tới khi chỉ còn 1 nút gốc.

🔹 Bước 4: Tạo mã Huffman cho từng phần tử

Ví dụ:

Giá trị	Mã Huffman
0	0
5	10
-3	110
2	111
-1	101
🔹 Bước 5: Mã hóa toàn bộ dãy

→ Output bitstream:

10 110 111 101 0 ...