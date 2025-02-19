#include <QueryPlan/ExplainAnalyzeStep.h>
#include <DataTypes/DataTypeString.h>
#include <Processors/QueryPipeline.h>
#include <Processors/Transforms/ExplainAnalyzeTransform.h>

#include <utility>

namespace DB
{


ExplainAnalyzeStep::ExplainAnalyzeStep(
    const DataStream & input_stream_,
    ASTExplainQuery::ExplainKind kind_,
    ContextMutablePtr context_,
    std::shared_ptr<QueryPlan> query_plan_ptr_,
    bool print_stats_,
    bool print_profile_
    )
    : ITransformingStep(std::move(input_stream_), {{std::make_shared<DataTypeString>(),"Explain Analyze"}}, {})
    , kind(kind_)
    , context(context_)
    , query_plan_ptr(query_plan_ptr_)
    , print_stats(print_stats_)
    , print_profile(print_profile_)
{
}

void ExplainAnalyzeStep::transformPipeline(QueryPipeline & pipeline, const BuildQueryPipelineSettings & )
{
    if (!query_plan_ptr)
        throw Exception("QueryPlan is not set", ErrorCodes::LOGICAL_ERROR);
    pipeline.resize(1);
    pipeline.addSimpleTransform([&](const Block & header) { return std::make_shared<ExplainAnalyzeTransform>(header, kind, query_plan_ptr, context, segment_descriptions, print_stats, print_profile); });
}

void ExplainAnalyzeStep::setInputStreams(const DataStreams & input_streams_)
{
    input_streams = input_streams_;
    output_stream->header = Block{{std::make_shared<DataTypeString>(),"Explain Analyze"}};
}

std::shared_ptr<IQueryPlanStep> ExplainAnalyzeStep::copy(ContextPtr) const
{
    return std::make_shared<ExplainAnalyzeStep>(input_streams[0], kind, context, query_plan_ptr, print_stats, print_profile);
}

}
