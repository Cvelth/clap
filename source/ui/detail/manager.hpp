#pragma once
namespace clap::ui {
	class zone;
	namespace detail {
		class manager {
		public:
			static void add(ui::zone &zone_ref);
			static void remove(ui::zone &zone_ref);
			static bool empty();
			static void wait();
		public:
			inline static void update() {
				update_additions();
				update_removals();
			}
		protected:
			static void update_additions();
			static void update_removals();
		};
	}
}