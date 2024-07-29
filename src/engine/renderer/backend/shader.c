#include "shader.h"

VkShaderModule bcknd_create_shader_module(const char* path, device* device) {
    VkShaderModule mod;

    size_t size = 0;
    char* code = READ_SHADER_FILE(path, &size);

    VkShaderModuleCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = size,
        .pCode = (uint32_t*) code 
    };
    VK_CHECK(vkCreateShaderModule(device->logical, &info, 0, &mod))

    return mod;
}


void bcknd_destroy_shader_module(VkShaderModule* mod, device* device) {
    vkDestroyShaderModule(device->logical, *mod, 0);
}

void bcknd_create_graphics_shader_pipeline(VkPipeline* pipe, 
        VkPipelineLayout lay, 
        device* device, 
        renderpass* pass, 
        VkShaderModule vertMod, 
        VkShaderModule fragMod, 
        VkExtent2D extent) {

    VkPipelineInputAssemblyStateCreateInfo asmState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .primitiveRestartEnable = VK_FALSE,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
    };

    // VkVertexInputBindingDescription bindingDesc = get_vertex_binding_desc();
    // VkVertexInputAttributeDescription* descs = get_vertex_attrib_desc();
    // uint32_t descsCount = ARR_SIZE(descs);
    VkPipelineVertexInputStateCreateInfo vertState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO//,
        // .vertexBindingDescriptionCount = 1,
        // .pVertexBindingDescriptions = &bindingDesc,
        // .vertexAttributeDescriptionCount = descsCount,
        // .pVertexAttributeDescriptions = descs
    };

    VkPipelineShaderStageCreateInfo stages[2];
    stages[0] = (VkPipelineShaderStageCreateInfo) {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_VERTEX_BIT,
        .module = vertMod,
        .pName = "main"
    };

    stages[1] = (VkPipelineShaderStageCreateInfo) {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = fragMod,
        .pName = "main"
    };

    VkDynamicState dynamicStates[2] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = ARR_SIZE(dynamicStates),
        .pDynamicStates = dynamicStates
    };

    VkRect2D sc = {
        .offset = { 0, 0 },
        .extent = extent
    };

    VkViewport vp = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float) extent.width,
        .height = (float) extent.height,
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    };

    VkPipelineViewportStateCreateInfo vpState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .scissorCount = 1,
        .pScissors = &sc,
        .viewportCount = 1,
        .pViewports = &vp,
    };

    VkPipelineRasterizationStateCreateInfo rasState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .lineWidth = 1.0f,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .frontFace = VK_FRONT_FACE_CLOCKWISE
    };

    VkPipelineMultisampleStateCreateInfo multiSmplState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .sampleShadingEnable = VK_FALSE,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .minSampleShading = 1.0f
    };

    VkPipelineColorBlendAttachmentState colBlndAttach = {
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
        .blendEnable = VK_TRUE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD
    };

    VkPipelineColorBlendStateCreateInfo colBlndState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .blendConstants[0] = 0.0f,
        .blendConstants[1] = 0.0f,
        .blendConstants[2] = 0.0f,
        .blendConstants[3] = 0.0f,
        .attachmentCount = 1,
        .pAttachments = &colBlndAttach,
        .logicOp = VK_LOGIC_OP_COPY,
        .logicOpEnable = VK_FALSE
    };
    
    VkGraphicsPipelineCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .basePipelineHandle = VK_NULL_HANDLE,
        .layout = lay,
        .renderPass = pass->pass,
        .pInputAssemblyState = &asmState,
        .pVertexInputState = &vertState,
        .stageCount = ARR_SIZE(stages),
        .pStages = stages,
        .pDynamicState = &dynamicState,
        .pViewportState = &vpState,
        .pRasterizationState = &rasState,
        .pMultisampleState = &multiSmplState,
        .pColorBlendState = &colBlndState
    };
    
    VK_CHECK(vkCreateGraphicsPipelines(device->logical, VK_NULL_HANDLE, 1, &info, 0, pipe));
}

void bcknd_destroy_shader_pipeline(device* device, VkPipeline* pipe) {
    vkDestroyPipeline(device->logical, *pipe, 0);
}