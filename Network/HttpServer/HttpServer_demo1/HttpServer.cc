#include "HttpServer.hpp"
#include <memory>

using namespace server;

void Usage(std::string proc)
{
    std::cerr << "Usage:\n\t" << proc << " port\r\n\r\n";
}

bool Get(const HttpRequest &req, HttpResponse &resp)
{
    // for test
    std::cout << "-------------------Http start---------------------" << std::endl;
    std::cout << req.get_inbuffer() << std::endl;
    std::cout << "method: " << req.get_method() << std::endl;
    std::cout << "url: " << req.get_url() << std::endl;
    std::cout << "httpversion: " << req.get_httpversion() << std::endl;
    std::cout << "path: " << req.get_path() << std::endl;
    std::cout << "-------------------Http end---------------------" << std::endl;

    std::string respline = "HTTP/1.1 200 OK\r\n";
    std::string respheader = "Content-Typpe:text/html\r\n";
    std::string respblank = "\r\n";
    std::string body = "<!DOCTYPE html><html lang=\"zh-CN\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=devicelixin1119876462-width, initial-scale=1.0\"><title>毛星云 - 生平与评价</title><style>body {font-family: 'Microsoft YaHei', Arial, sans-serif;line-height: 1.6;max-width: 800px;margin: 0 auto;padding: 20px;color: #333;background-color: #f9f9f9;}h1 {color: #2c3e50;text-align: center;margin-bottom: 30px;border-bottom: 2px solid #3498db;padding-bottom: 10px;}h2 {color: #2980b9;margin-top: 30px;border-left: 4px solid #3498db;padding-left: 10px;}.profile {display: flex;margin-bottom: 30px;align-items: center;}.profile-img {width: 150px;height: 150px;border-radius: 50%;object-fit: cover;margin-right: 20px;border: 3px solid #3498db;}.profile-info {flex: 1;}.timeline {position: relative;padding-left: 30px;margin-bottom: 30px;}.timeline::before {content: '';position: absolute;left: 6px;top: 0;bottom: 0;width: 2px;background: #3498db;}.timeline-item {position: relative;margin-bottom: 20px;padding-bottom: 20px;border-bottom: 1px solid #eee;}.timeline-item::before {content: '';position: absolute;left: -30px;top: 5px;width: 12px;height: 12px;border-radius: 50%;background: #3498db;}.timeline-year {font-weight: bold;color: #3498db;}.quote {background: #e8f4fc;border-left: 4px solid #3498db;padding: 15px;margin: 20px 0;font-style: italic;}.quote-source {text-align: right;font-weight: bold;margin-top: 10px;}.works-list {list-style-type: none;padding: 0;}.works-list li {padding: 10px;margin-bottom: 10px;background: #fff;border-radius: 4px;box-shadow: 0 2px 4px rgba(0,0,0,0.1);}.footer {text-align: center;margin-top: 40px;padding-top: 20px;border-top: 1px solid #eee;color: #7f8c8d;font-size: 0.9em;}</style></head><body><h1>毛星云 (1991-2021)</h1><div class=\"profile\"><img src=\"https://via.placeholder.com/150\" alt=\"毛星云照片\" class=\"profile-img\"><div class=\"profile-info\"><p><strong>网络ID:</strong> 浅墨</p><p><strong>出生地:</strong> 湖北宜昌</p><p><strong>毕业院校:</strong> 南京航空航天大学</p><p><strong>职业:</strong> 程序员、专栏作家、游戏引擎开发专家</p><p><strong>主要成就:</strong> 微软MVP、出版多本编程书籍、腾讯游戏引擎组组长</p></div></div><h2>生平事迹</h2><div class=\"timeline\"><div class=\"timeline-item\"><span class=\"timeline-year\">1991年</span> - 出生于湖北宜昌。</div><div class=\"timeline-item\"><span class=\"timeline-year\">2009年</span> - 考入南京航空航天大学\"中国乌克兰航天联合培养班\"。</div><div class=\"timeline-item\"><span class=\"timeline-year\">2011年</span> - 赴乌克兰国立茹科夫斯基航空航天大学就读，留学期间完成100余万字的《逐梦旅程：Windows游戏编程之从零开始》一书:cite[1]:cite[7]。</div><div class=\"timeline-item\"><span class=\"timeline-year\">2013年</span> - 《逐梦旅程》由清华大学出版社出版，成为游戏编程入门经典:cite[1]。</div><div class=\"timeline-item\"><span class=\"timeline-year\">2014年</span> - 荣获微软\"最有价值专家\"(MVP)称号，是中国区唯一的90后获奖者:cite[1]:cite[6]。</div><div class=\"timeline-item\"><span class=\"timeline-year\">2015年</span> - 出版《OpenCV3编程入门》，成为计算机视觉领域重要参考书:cite[1]。</div><div class=\"timeline-item\"><span class=\"timeline-year\">2015年</span> - 加入腾讯IEG天美工作室群，担任游戏引擎开发工程师:cite[3]。</div><div class=\"timeline-item\"><span class=\"timeline-year\">2021年</span> - 晋升为腾讯互娱天美工作室群引擎组组长(Lead Engine Programmer):cite[3]。</div><div class=\"timeline-item\"><span class=\"timeline-year\">2021年10月</span> - 在知乎宣布腾讯天美成立\"F1工作室\"，主攻3A大作开发:cite[3]:cite[8]。</div><div class=\"timeline-item\"><span class=\"timeline-year\">2021年12月11日</span> - 不幸离世，年仅30岁:cite[1]:cite[3]。</div></div><h2>主要著作</h2><ul class=\"works-list\"><li><strong>《逐梦旅程:Windows游戏编程之从零开始》</strong><br>2013年，清华大学出版社<br>一本百万字的游戏编程入门指南，帮助无数开发者入门游戏编程:cite[1]:cite[8]。</li><li><strong>《OpenCV3编程入门》</strong><br>2015年，电子工业出版社<br>计算机视觉领域的实用指南，被广泛引用和学习:cite[1]:cite[7]。</li><li><strong>《Real-Time Rendering 4th》中译版</strong><br>未完成，正在翻译中:cite[1]:cite[8]。</li></ul><h2>梦想与贡献</h2><p>毛星云有一个宏大的梦想：\"有一天，不少3A大作不需要汉化，因为是我们自己的游戏，配音是中文，文化也是中国的。\":cite[4]:cite[8]</p><p>他在书中写道：\"我有一个梦想，将来的某一天，国产游戏能像中国的其他产业一样，以一个领跑者的姿态，面对全世界，面对全宇宙，器宇轩昂，扬眉吐气。\":cite[8]</p><p>毛星云以\"浅墨\"为ID活跃在CSDN等IT社区多年，博客访问量突破170万次，2013年被评为\"中国年度十大杰出IT博客作者\":cite[6]:cite[8]。</p><h2>他人评价</h2><div class=\"quote\">\"单论图像引擎的开发能力，能达到毛星云水平的，在国内，一只手都能数得过来。而如果论毛星云对整个行业的贡献，恐怕在国内，无人能与之匹敌。\"<div class=\"quote-source\">— 游戏业内引擎开发总监:cite[2]:cite[4]</div></div><div class=\"quote\">\"在图形算法的掌握上，毛星云一人比我一个团队都要强。\"<div class=\"quote-source\">— 游戏公司创始人兼CTO:cite[2]:cite[4]</div></div><div class=\"quote\">\"能具备毛星云这样非常好的底子（硕士毕业，国外留学），达到毛星云在游戏引擎的资历（从PC端游DirectX到OpenGL手游涵盖）和能力程度（从基础图形到各种高级次时代光效算法）的，在中国一个巴掌数的过来，但如果仔细去看毛星云本身的资质和对于行业输出的贡献（出版读物和文章），则几乎无人出其右。\"<div class=\"quote-source\">— 资深游戏行业专家:cite[8]</div></div><div class=\"quote\">\"毛星云大神为人温和谦逊，有着特殊的文艺气息。与他交谈往往如沐春风，完全没有架子，讲话慢条斯理。\"<div class=\"quote-source\">— 同事评价:cite[8]</div></div><div class=\"quote\">\"大神相貌清秀，曾得到南航团宣老师的表扬'像花样美男一样帅'。大神的穿戴也很注重生活品质。他的衣着不浮夸，不低俗，看他的衣着就如同他的人，温润如玉，很舒服，很清新。\"<div class=\"quote-source\">— 朋友回忆:cite[8]</div></div><h2>个人生活</h2><p>毛星云不仅是一位技术天才，在生活中也是一个多才多艺的人：</p><ul><li>热爱烹饪，擅长做辣子鸡、口水肉片、干煸牛肉等菜肴:cite[8]</li><li>喜欢小动物，养有一只名为\"丢丢\"的比熊犬:cite[8]</li><li>注重生活品质，电子设备总是最新款:cite[8]</li><li>已婚并育有两个孩子:cite[6]:cite[8]</li></ul><div class=\"footer\"><p>纪念毛星云 (1991-2021)</p><p>资料来源于公开报道和网络资源</p></div></body></html>";
    resp.get_outbuffer() += respline;
    resp.get_outbuffer() += respheader;
    resp.get_outbuffer() += respblank;
    resp.get_outbuffer() += body;
    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(0);
    }
    uint16_t port = atoi(argv[1]);
    HttpServer myserver(Get, port);
    myserver.initServer();
    myserver.start();
    return 0;
}