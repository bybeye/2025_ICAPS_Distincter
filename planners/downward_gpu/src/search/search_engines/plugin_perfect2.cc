#include "perfect2.h"
#include "search_common.h"

#include "../plugins/plugin.h"

using namespace std;

namespace plugin_perfect2 {
class PerfectFeature2 : public plugins::TypedFeature<SearchEngine, perfect2::Perfect2> {
public:
    PerfectFeature2() : TypedFeature("perfect2") {
        document_title("perfect2");
        document_synopsis("convert plan to states");

        add_list_option<shared_ptr<Evaluator>>("evals", "evaluators");
        add_list_option<shared_ptr<Evaluator>>(
            "preferred",
            "use preferred operators of these evaluators", "[]");
        add_option<int>(
            "boost",
            "boost value for preferred operator open lists", "0");
        perfect2::add_options_to_feature(*this);

        document_note(
            "Open list",
            "In most cases, eager greedy best first search uses "
            "an alternation open list with one queue for each evaluator. "
            "If preferred operator evaluators are used, it adds an extra queue "
            "for each of these evaluators that includes only the nodes that "
            "are generated with a preferred operator. "
            "If only one evaluator and no preferred operator evaluator is used, "
            "the search does not use an alternation open list but a "
            "standard open list with only one queue.");
        document_note(
            "Closed nodes",
            "Closed node are not re-opened");
        document_note(
            "Preferred operators",
            "Not implemented yet");
    }
};

static plugins::FeaturePlugin<PerfectFeature2> _plugin;
}
