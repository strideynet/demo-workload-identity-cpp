// Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/ListObjectsV2Request.h>
#include <aws/s3/model/Object.h>
#include <aws/sts/STSClient.h>
#include <aws/sts/model/GetCallerIdentityRequest.h>
#include <aws/sts/model/GetCallerIdentityResult.h>
#include <iostream>
using namespace Aws;
using namespace Aws::Auth;

int main(int argc, char **argv) {
    Aws::SDKOptions options;
    Aws::InitAPI(options); // Should only be called once.
    
    Aws::Client::ClientConfiguration clientConfig; 

    Aws::STS::STSClient stsClient(clientConfig);

    // Create a request to get the caller identity
    Aws::STS::Model::GetCallerIdentityRequest identityRequest;
    // Get the caller's identity
    auto outcome = stsClient.GetCallerIdentity(identityRequest);
    if (outcome.IsSuccess()) {
        const Aws::STS::Model::GetCallerIdentityResult& identityResult = outcome.GetResult();
        std::cout << "Account ID: " << identityResult.GetAccount() << std::endl;
        std::cout << "User ID: " << identityResult.GetUserId() << std::endl;
        std::cout << "Arn: " << identityResult.GetArn() << std::endl;
    }
    else {
        std::cout << "Failed to get caller identity: " << outcome.GetError().GetMessage() << std::endl;
    }

    const char* bucketName = "noah-workload-id-demo";
    Aws::S3::S3Client s3Client(clientConfig);
    Aws::S3::Model::ListObjectsV2Request objectsRequest;
    objectsRequest.WithBucket(bucketName);

    // Get the list of objects in the bucket
    auto s3Outcome = s3Client.ListObjectsV2(objectsRequest);
    if (s3Outcome.IsSuccess()) {
        auto objects = s3Outcome.GetResult().GetContents();
        for (const auto& object : objects) {
            std::cout << "Object Key: " << object.GetKey() << " Size: " << object.GetSize() << std::endl;
        }
    }
    else {
        std::cout << "Failed to list objects in S3 bucket: " << s3Outcome.GetError().GetMessage() << std::endl;
    }

    Aws::ShutdownAPI(options); // Should only be called once.
    return 0;
}
