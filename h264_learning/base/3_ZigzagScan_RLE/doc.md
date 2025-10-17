Mục tiêu:

Biểu diễn các hệ số DCT (sau lượng tử hóa) theo thứ tự zigzag (chuẩn JPEG).

Thực hiện mã hóa Run-Length Encoding cho dãy zigzag.

In kết quả ra file CSV để thấy rõ cách dữ liệu được nén.

Cụ thể:

Viết hàm zigzagScan(int block[8][8], int output[64]).

Viết hàm runLengthEncode(int input[64], FILE *csv) – xuất (run, value) ra file.

Áp dụng với mỗi block DCT lượng tử hóa từ bài trước.

Kết quả:

File zigzag_rle.csv lưu toàn bộ dữ liệu sau RLE.

So sánh kích thước dữ liệu trước/sau RLE.










Zigzag làm gì?
JPEG đọc 8×8 theo đường chéo hình “Z” để gom các hệ số quan trọng (thấp tần) ở đầu, còn các hệ số cao tần (thường bằng 0) về cuối.
Gom nhóm các giá trị 0 liên tiếp ở cuối mảng → cực kỳ thuận lợi cho bước nén kế tiếp.

--------------

Run-Length Encoding (RLE) — “Nén dãy số 0”
Sau Zigzag, ta có một mảng 64 phần tử:
[DC, a1, a2, a3, 0, 0, 0, 0, 0, 0, 0, aN]

RLE thay dãy “0” lặp lại bằng (số lượng zero, giá trị tiếp theo).
[10, 5, -3, 0, 0, 0, 2, 0, 0, 0, 0, 1]
→ [(0,10), (0,5), (0,-3), (3,2), (4,1), (EOB)]
(EOB = End Of Block)

🔍 Ý nghĩa thực tế
Bước	                    Mục tiêu	                        Giúp ích
Zigzag	                    Gom hệ số 0 về cuối	                Giảm độ dài dữ liệu có ý nghĩa
RLE	                        Biểu diễn chuỗi 0 bằng gọn hơn	    Tiết kiệm không gian
→ Huffman coding (sau này)	Mã hóa tần suất bit	                Giảm thêm kích thước


--------------


Frame Y
 ↓
Tách 8×8 Block
 ↓
DCT
 ↓
Quantization
 ↓
Zigzag
 ↓
Run-Length Encoding
 ↓
Huffman Coding
 ↓
→ JPEG Stream