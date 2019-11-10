
#pragma once

#include "exception/exception.hxx"

#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TTree.h"

#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

// #define ROOTUILS_NOTIFYFILEOP

namespace nft {
namespace utils {

NEW_NFT_EXCEPT(failed_to_open_TFile);
NEW_NFT_EXCEPT(failed_to_get_TTree);
NEW_NFT_EXCEPT(WriteToTFile_nullptr);

inline void CloseTFile(TFile *f = nullptr) {
  if (f) {
#ifdef ROOTUILS_NOTIFYFILEOP
    std::cout << "[INFO]: Shutting TFile: " << f->GetName() << ", " << f
              << std::endl;
#endif
    f->Close();
  }
  delete f;
}

using TFile_ptr = std::unique_ptr<TFile, decltype(&CloseTFile)>;
inline TFile_ptr make_unique_TFile(TFile *f) {
  return TFile_ptr(f, &CloseTFile);
}

inline TFile_ptr CheckOpenTFile(std::string const &fname,
                                char const *opts = "") {

  TDirectory *ogDir = gDirectory;

  TFile *inpF = new TFile(fname.c_str(), opts);
  if (!inpF || !inpF->IsOpen()) {
    throw failed_to_open_TFile()
        << "[ERROR]: Couldn't open input file: " << std::quoted(fname);
  }
#ifdef ROOTUILS_NOTIFYFILEOP

  std::cout << "[INFO]: Opened TFile " << fname << ", " << inpF << std::endl;
#endif
  if (ogDir) {
    ogDir->cd();
  }

  return make_unique_TFile(inpF);
}

struct TreeFile {
  TFile_ptr file;
  TTree *tree;
  bool file_owned;
  TreeFile()
      : file(make_unique_TFile(nullptr)), tree(nullptr), file_owned(false) {}
  TreeFile(TreeFile const &other) = delete;
  TreeFile &operator=(TreeFile &&other) {
    file = std::move(other.file);
    tree = other.tree;
    file_owned = other.file_owned;

    other.file = nullptr;
    other.tree = nullptr;
    other.file_owned = false;

    return *this;
  }

  TreeFile(TreeFile &&other)
      : file(std::move(other.file)), tree(other.tree),
        file_owned(other.file_owned) {
    other.file = nullptr;
    other.tree = nullptr;
    other.file_owned = false;
  }

  TTree *operator->() { return tree; }

  ~TreeFile() {
    if (file && file_owned) {
      file->Write();
    }
  }
};

inline TreeFile CheckGetTTree(TFile *file, std::string const &tname) {
  TreeFile tf;
  tf.tree = dynamic_cast<TTree *>(file->Get(tname.c_str()));

  if (!tf.tree) {
    throw failed_to_get_TTree()
        << "[ERROR]: Failed to get TTree named: " << std::quoted(tname)
        << " from TFile: " << std::quoted(file->GetName());
  }
#ifdef ROOTUILS_NOTIFYFILEOP

  std::cout << "[INFO]: Opened TTree: " << std::quoted(tname)
            << ", from file: " << file->GetName() << ", " << file << std::endl;
#endif
  return tf;
}

inline TreeFile CheckGetTTree(std::string const &fname,
                              std::string const &tname, char const *opts = "") {
  TFile_ptr file = CheckOpenTFile(fname, opts);
  TreeFile tf = CheckGetTTree(file.get(), tname);
  tf.file = std::move(file);
  tf.file_owned = true;
  return tf;
}

inline TreeFile MakeNewTTree(std::string const &fname, std::string const &tname,
                             char const *opts = "") {
  TreeFile tf;
  tf.file = CheckOpenTFile(fname, opts);
  tf.tree = new TTree(tname.c_str(), "");
  tf.tree->SetDirectory(tf.file.get());
  tf.file_owned = true;
  return tf;
}

inline TreeFile AddNewTTreeToFile(TFile *f, std::string const &tname,
                                  std::string const &dirname = "") {
  TreeFile tf;
  // This is pretty dumb...
  tf.file = nullptr;
  tf.tree = new TTree(tname.c_str(), "");
  if (dirname.size()) {
    TDirectory *dir = f->GetDirectory(dirname.c_str());
    if (!dir) {
      dir = f->mkdir(dirname.c_str());
    }
    tf.tree->SetDirectory(dir);
  } else {
    tf.tree->SetDirectory(f);
  }
  tf.file_owned = false;
  return tf;
}

inline std::string SanitizeROOTObjectName(std::string name) { return name; }

inline void WriteToTFile(TFile_ptr &tf, TObject *object,
                         std::string const &object_name) {
  if (!object) {
    throw WriteToTFile_nullptr();
  }

  TDirectory *ogdir = gDirectory;

  tf->WriteTObject(object, SanitizeROOTObjectName(object_name).c_str(),
                   "overwrite");

  if (ogdir) {
    ogdir->cd();
  }
}

} // namespace utils
} // namespace nft
