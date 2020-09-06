#include "window.hpp"
#include "resource/resource.hpp"

#include <chrono>
#include <thread>

//FORCE_NVIDIA_GPU_ON_OPTIMUS;

void window::initialize() {
	using namespace clap::gl;

	clap::resource::load();
	clear::set_color(0.4f, 0.0f, 0.5f, 1.0f);


	text_program.add(clap::resource::shader::fragment["text"],
					 clap::resource::shader::vertex["text"],
					 clap::resource::shader::geometry["text"]);
	text_program.link();
	variables = text_program.getVariables();

	clap::gl::enable::blend(clap::gl::blend_function::source_alpha);
	text = std::make_unique<clap::render::text>(u8"A simple example. 日本語もここだよ!",
												clap::resource::font["GL-AntiquePlus"],
												text_program, 48);
	long_text = std::make_unique<clap::render::text>(
		u8"This text string is super long.\n"
		u8"It's used to test text renderer more thoroughly, with quite a bit more data, characters and such.\n"
		u8"The quick brown fox jumps over the lazy dog.\n"
		u8"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
		u8"Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n"
		u8"Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\n"
		u8"Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n",
		clap::resource::font["Kalam-Light"],
		text_program, 24
	);
	japanese_text = std::make_unique<clap::render::text>(
		u8"ろぬふあうえおやゆよわほほほへへへへへへたていすかんなにらぜ゜むけれりのまくきはしとちつさそひこみもねるめ!"
		u8"\n[32]しかし私は、喜びを非難して苦痛を賞賛するという誤ったこの考えがすべてどのようにして誕生したかをあなたに説明しなければならないから、"
		u8"\n私はあなたにその体系を完璧に説明し、真実を求める偉大な探究家、人間の喜びを築く建築家の実践的な教えを詳しく説明しよう。だれも喜びそのものを、"
		u8"\nそれが喜びであるという理由で拒んだり、嫌ったり、避けたりはしない。しかし、どのようにして喜びを理性的に追求するかを知らない人たちは非常に苦痛な結末に直面する。"
		u8"\n同様に、苦痛そのものを、それが苦痛であるという理由で愛したり、探したり、手に入れることを望んだりする者もいない。"
		u8"\nしかし、ときには苦労や苦痛がその人に大いなる喜びをいくらかもたらす状況がおこることがある。些末な例を挙げると、"
		u8"\n私たちのうちのだれが、そこから何か有益なものを得られないのに、骨の折れる肉体運動を引き受けるだろうか？しかしだれに、"
		u8"\nいらだたしい結末のない喜びを享受することを選ぶ人や、その結果としての喜びを生み出さないような痛みを避ける人にある、落ち度を見つける権利はあるのだろうか？"
		u8"\n[33]一方、わたしたちは正当な憤りをもって批判する。そして、今の喜びの魅力にだまされてあまりにもやる気を失い、"
		u8"\n欲望によってあまりにも盲目的になってしまうことで、その後に起こるべき苦痛や困難を予想できなくなってしまう人たちを嫌う；"
		u8"\nそして、同等の非難は意志の弱さによって自身の務めに失敗する人たちにも当てはまる。これは苦労から苦痛へと縮小することと等しい。"
		u8"\nこれらの場合は完全に単純で、見分けるのはたやすい。自由なときにおいて、選択の力に制約がなく、いちばん好きなものを選ぶのに一切の妨げがないとき、"
		u8"\nあらゆる喜びが受け入れられ、あらゆる苦痛が避けられる。しかし、ある特定の状況において務めや義務により、しばしば喜びを拒み、"
		u8"\nいらだたしいことを受け入れなければならないことが起こる。賢い人間はそれゆえ常に、この選択の原則によってこれらの問題を制する："
		u8"\n賢い人間は他のより大きな喜びを確実に手に入れるためならば喜びを拒み、あるいはよりひどいものを避けるために苦痛に耐える。"
		u8"\n三島事件とは、1970年（昭和45年）11月25日に、日本の作家・三島由紀夫が、憲法改正のため自衛隊の決起（クーデター）を呼びかけた後に割腹自殺をした事件である。"
		u8"\n三島と同じ団体「楯の会」のメンバーも事件に参加したことから、その団体の名前をとって楯の会事件とも呼ばれる……"
		u8"\nこまねこまつりは、京都府京丹後市峰山町金刀比羅神社と同町市街地で行われる祭り。"
		u8"\n金刀比羅神社境内末社木島神社にある狛猫を核に据えた地域おこしをめざす町民らによって発足した"
		u8"\n「ねこプロジェクト」の発展として始まったまちあるきイベント……"
		u8"\nドイツ共産党は、かつて存在したドイツの共産主義政党。第一次世界大戦中の「スパルタクス団」を起源とし、"
		u8"\nドイツ革命直後に結党され、ヴァイマル共和政期にナチ党、社会民主党に次ぐ第三党まで上り詰めたが、"
		u8"\n1933年にナチ党が政権を獲得すると禁止された……",
		clap::resource::font["GL-AntiquePlus"],
		text_program, 16
	);

	counter_label = std::make_unique<clap::render::text>("Counter: ",
														 clap::resource::font["Kalam-Light"],
														 text_program, 24);
	counter = std::make_unique<clap::render::text>("0",
												   clap::resource::font["Kalam-Light"],
												   text_program, 24);
	counter_value = 0;

	std::thread counter_thread(
		[this]() {
			while (!should_close()) {
				auto time = std::chrono::high_resolution_clock::now() + std::chrono::seconds(1);
				counter_value++;
				update();
				std::this_thread::sleep_until(time);
			}
		}
	);
	counter_thread.detach();
}

void window::render() {
	using namespace clap::gl;
	auto time = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(10);

	poll_events();
	if (should_update()) {
		clear::color();

		text->draw(width() / 8, height() / 20);
		long_text->draw(0, height() / 6);
		japanese_text->draw(0, height() / 2);

		counter->update(std::to_string(counter_value));
		counter_label->draw(0, height() / 2 - 70);
		counter->draw(100, height() / 2 - 70);

		redraw();
	}

	std::this_thread::sleep_until(time);
}

void window::cleanup() {
	using namespace clap::gl;

	clap::resource::clear();
}

bool window::on_resize(size_t width, size_t height) {
	using namespace clap::gl;

	float projection_matrix[] = {
		2.f / width,	0.f,			0.f,	0.f,
		0.f,			-2.f / height,	0.f,	0.f,
		0.f,			0.f,			-1.f,	0.f,
		-1.f,			1.f,			0.f,	1.f
	};

	text_program.set(variables["projection_matrix"], projection_matrix);
	update();

	return true;
}
