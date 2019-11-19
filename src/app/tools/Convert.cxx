#include "app/tools/INFTTool.hxx"

#include "plugins/Instantiate.hxx"

#include "flux/IDecayParentReader.hxx"
#include "flux/converters/IFluxConverter.hxx"

#include "geom/CoordinateTransformation.hxx"

#include "TRandom3.h"

#include "fhiclcpp/make_ParameterSet.h"

class Convert : public INFTTool {

  void SayUsage() { std::cout << "HALP" << std::endl; }

  std::string fhicl_config;
  std::vector<std::string> Inputs;

  void handleOpts(std::vector<std::string> const &args) {
    bool got_fhicl_config;
    for (size_t opt = 1; opt < args.size();) {
      if (std::string(args[opt]) == "-?" ||
          std::string(args[opt]) == "--help") {
        SayUsage();
        exit(0);
      } else if (std::string(args[opt]) == "-f") {
        fhicl_config = args[++opt];
      } else if (std::string(args[opt]) == "-i") {
        Inputs.push_back(args[++opt]);
      } else {
        std::cout << "[ERROR]: Unknown option: " << args[opt] << std::endl;
        SayUsage();
        exit(1);
      }
      opt++;
    }
  }

public:
  bool Run(std::vector<std::string> const &args) {

    fhicl_config = "";
    handleOpts(args);
    if (!fhicl_config.size()) {
      throw nft::bad_config()
          << "Convert tool didn't receive an fhicl configuration. Pass with -f "
             "after the tool name.";
    }
    if (!Inputs.size()) {
      throw nft::bad_config() << "Convert tool didn't recieve any inputs, pass "
                                 "with -i after the tool name.";
    }

    fhicl::ParameterSet conf = fhicl::make_ParameterSet(fhicl_config);

    auto tgs = nft::plugins::Instantiate<IFluxConverter>(
        conf.get<std::string>("convert.plugin"));

    gRandom->SetSeed(conf.get<size_t>("gseed", 0));

    tgs->Initialize(conf.get<fhicl::ParameterSet>("convert"));

    auto dkreader = nft::plugins::Instantiate<IDecayParentReader>(
        conf.get<std::string>("input.reader"));

    dkreader->Initialize(Inputs.front());
    if (Inputs.size() > 1) {
      for (size_t i = 1; i < Inputs.size(); ++i) {
        dkreader->AddFiles(Inputs[i]);
      }
    }

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
