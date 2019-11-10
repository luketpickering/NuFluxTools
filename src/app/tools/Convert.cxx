#include "app/tools/INFTTool.hxx"

#include "plugins/Instantiate.hxx"

#include "flux/IDecayParentReader.hxx"
#include "flux/converters/IFluxConverter.hxx"

#include "geom/CoordinateTransformation.hxx"

#include "TRandom3.h"

#include "fhiclcpp/make_ParameterSet.h"

class Convert : public INFTTool {
public:
  bool Run(std::vector<std::string> const &args) {
    fhicl::ParameterSet conf = fhicl::make_ParameterSet(args[1]);

    auto tgs = nft::plugins::Instantiate<IFluxConverter>(
        conf.get<std::string>("convert.plugin"));

    gRandom->SetSeed(conf.get<size_t>("gseed", 0));

    tgs->Initialize(conf.get<fhicl::ParameterSet>("convert"));

    auto dkreader = nft::plugins::Instantiate<IDecayParentReader>(
        conf.get<std::string>("input.reader"));

    dkreader->Initialize(conf.get<std::string>("input.file"));

    auto tf = nft::geom::BuildTransformation(conf.get<fhicl::ParameterSet>(
        "convert.beam_to_det_transformation", {}));

    dkreader->SetTransformation(tf.Inverse());

    double req_POT = conf.get<double>("convert.POTToProcess");
    int max_reuse = conf.get<int>("convert.MaxReuse");
    double POT_consumed = 0;

    double POT_shout_ratio = 1000;
    double next_POT_shout = req_POT / POT_shout_ratio;

    std::cout << "[INFO]: Converting at least " << req_POT << " POT."
              << std::endl;
    int reuse = 0;
    while ((reuse < max_reuse) && (POT_consumed < req_POT)) {
      for (auto dk : *dkreader) {
        POT_consumed += tgs->Convert(dk);
        if (POT_consumed >= req_POT) {
          break;
        }
      }
      if (POT_consumed > next_POT_shout) {
        std::cout << "\r[INFO]: Parsed " << POT_consumed << " POT"
                  << std::flush;
        next_POT_shout += req_POT / POT_shout_ratio;
      }
      if (POT_consumed >= req_POT) {
        break;
      }
      reuse++;
    }
    std::cout << std::endl;
    std::cout << "[INFO]: Converted " << POT_consumed << " POT, which used "
              << tgs->GetEntriesUsed() << " entries (" << reuse
              << " loops through the input file)." << std::endl;
    std::cout << (tgs->GetEntriesUsed() / POT_consumed) << " Ray/POT ratio."
              << std::endl;
    return true;
  };
};

DECLARE_PLUGIN(INFTTool, Convert);
