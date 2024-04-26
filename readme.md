# Whack a usagi
- link preview game : [youtube](https://youtu.be/-1Npw2A5UbA?si=GNvPXeWHmDPLzrkK)
# Cài đặt
- bản chạy được luôn : tải daptho.rar và giải nén từ [release](https://github.com/NTDat05/daptho/releases/tag/release)
- bản chạy trên IDE hoặc textEditor : clone repo hoặc Code -> download zip </br> Nếu dùng Visual Studio thì có thể vào file sln và cài đặt SDL2 rồi Ctrl + F5 để chạy


# Nguồn 
Các hình ảnh trong game tự thiết kế hoặc tìm ngẫu nhiên trên google ngoại trừ :
- spriesheets thỏ : [vectorstock.com](https://www.vectorstock.com/)
- âm thanh :[myinstant.com]( https://www.myinstants.com/)
- font : [1001fonts.com](https://www.1001fonts.com/)
# Giới Thiệu Game
Chào mừng bạn đến với thế giới đầy màu sắc và trải nghiệm chơi game hấp dẫn hơn cùng trò chơi đập thỏ - một phiên bản sáng tạo và thú vị được lấy cảm hứng từ những kỷ niệm tuổi thơ về trò chơi đập chuột.

Trong trò chơi này, bạn sẽ không chỉ đối mặt với những chú thỏ đáng yêu mà còn phải đối đầu với những thách thức mới mẻ và kịch tính!

Một điểm khác biệt lớn của trò chơi là việc chú thỏ sẽ xuất hiện ngẫu nhiên trên màn hình, tạo ra một trải nghiệm chơi game hấp dẫn hơn bao giờ hết. Bạn sẽ không bao giờ biết chắc chắn chú thỏ tiếp theo sẽ xuất hiện ở đâu, khi nào, hoặc có màu sắc gì - điều này khiến cho trò chơi trở nên thú vị hơn và kích thích sự tò mò của bạn.

Ngoài ra, tốc độ xuất hiện và hoạt động của chú thỏ cũng sẽ tăng theo thời gian, đòi hỏi bạn phải phản xạ nhanh nhẹn và có chiến lược linh hoạt để đảm bảo không bỏ lỡ bất kỳ chú thỏ nào.

Đa dạng màu sắc của các chú thỏ cũng là một yếu tố khó khăn thêm vào trò chơi. Khả năng quan sát và nhận biết màu sắc sẽ là chìa khóa để bạn vượt qua các thách thức trong trò chơi này.

Với những điểm khác biệt này, trò chơi đập thỏ sẽ mang lại cho bạn một trải nghiệm chơi game độc đáo và hấp dẫn mà bạn không thể bỏ qua. Hãy chuẩn bị cho mình những phút giây giải trí sảng khoái và đầy tiếng cười cùng với Thỏ Nghịch!"


# Bắt đầu game
- Menu : có hai lựa chọn play hoặc quit
<div style="text-align: center;">

![menu](preview/menu.png)

</div>

- Rule : 
<div style="text-align: center;">

![rule](preview/rule.png)

</div>


- Chơi : 
<div style="text-align: center;">

![play](preview/play.png)

</div>

- kết thúc : có âm thanh phá kỉ lục và lựa chọn chơi lại
<div style="text-align: center;">

![gameover](preview/gameOver.png)

</div>


# Source code
- folder font: chứa font
- folder imgs: chứa hình ảnh, background, spritesheet,...
- folder sfx: hiệu ứng và âm thanh game
- main.cpp: chứa hàm main 
- - hàm showMenu(): hiển thị menu gồm tên game và hai nút play, quit
- - hàm showRule(): hiển thị luật chơi, nhấn esc để bắt đầu chơi 
- - hàm play (): chơi game, xử lý logic game, hiển thị điểm số, thời gian,...
- CommonFunc.cpp: khai báo thư viện và các hằng
- - định nghĩa hàm init để khởi tạo app
- - định nghĩa hàm loadMedia để tải dữ liệu
- - định nghĩa các hàm liên quan như kiểm tra va chạm
- - định nghĩa hàm close để xử lí hậu kì thoát chương trình
- Rabbit.cpp: chứa class Rabbit 
- - animate() : xử lí cho thỏ hiện lên lâu hơn chút 
