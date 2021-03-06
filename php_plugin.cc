/*
 *
 * Copyright 2016, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

// Generates PHP gRPC service interface out of Protobuf IDL.

#include <memory>

#include "config.h"
#include "php_generator.h"
#include "php_generator_helpers.h"

using grpc_php_generator::GenerateFile;
using grpc_php_generator::GetPHPServiceFilenamePrefix;

class PHPGrpcGenerator : public google::protobuf::compiler::CodeGenerator {
 public:
  PHPGrpcGenerator() {}
  ~PHPGrpcGenerator() {}

  bool Generate(const google::protobuf::FileDescriptor *file,
                const std::string &parameter,
                google::protobuf::compiler::GeneratorContext *context,
                std::string *error) const {
    if (file->service_count() == 0) {
      return true;
    }

    for (int i = 0; i < file->service_count(); i++) {
      {
        std::string code = GenerateFile(file, file->service(i), false);

        std::string file_name = GetPHPServiceFilenamePrefix(file, file->service(i));
        file_name += "Client.php";

        std::unique_ptr<google::protobuf::io::ZeroCopyOutputStream> output(
            context->Open(file_name));
        google::protobuf::io::CodedOutputStream coded_out(output.get());
        coded_out.WriteRaw(code.data(), code.size());
      }
      {
        std::string code = GenerateFile(file, file->service(i), true);

        std::string file_name = GetPHPServiceFilenamePrefix(file, file->service(i));
        file_name += ".php";

        std::unique_ptr<google::protobuf::io::ZeroCopyOutputStream> output(
            context->Open(file_name));
        google::protobuf::io::CodedOutputStream coded_out(output.get());
        coded_out.WriteRaw(code.data(), code.size());
      }
    }

    return true;
  }
};

int main(int argc, char *argv[]) {
  PHPGrpcGenerator generator;
  return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}
