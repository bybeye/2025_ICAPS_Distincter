#include "perfect3.h"
#include "search_common.h"

#include "../plugins/plugin.h"

using namespace std;

namespace plugin_perfect3 {
// class PerfectFeature3 : public plugins::TypedFeature<Evaluator, perfect3::Perfect3> {
class PerfectFeature3 : public plugins::TypedFeature<SearchEngine, perfect3::Perfect3> {
public:
    PerfectFeature3() : TypedFeature("perfect3") {
        document_title("perfect3");
        document_synopsis("convert plan to states");

        
        // add_list_option<shared_ptr<Evaluator>>("evals", "evaluators");
        // add_list_option<shared_ptr<Evaluator>>("preferred", "use preferred operators of these evaluators", "[]");
        
        //add_option<string>("plan_path", "plan file path", "default_value");
	//add_option<string>("states_path", "states file path", "default_value");
        // add_option<int>("boost", "boost value for preferred operator open lists", "0");

        perfect3::add_options_to_feature(*this);

    }
};

static plugins::FeaturePlugin<PerfectFeature3> _plugin;
}
