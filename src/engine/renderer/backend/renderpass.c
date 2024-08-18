#include "renderpass.h"

void create_renderpass(renderpass* pass, device* device, swapchain* sc) {
    VkAttachmentDescription colorAttachment = {
        .format = sc->format.format,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE
    };

    VkAttachmentDescription attachments[] = {
        colorAttachment
    };

    VkAttachmentReference colRef = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    VkAttachmentReference refs[] = {
        colRef
    };

    VkSubpassDescription subpass = {
        .colorAttachmentCount = ARR_SIZE(refs),
        .pColorAttachments = refs
    };

    VkSubpassDependency dependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
    };

    VkRenderPassCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = ARR_SIZE(attachments),
        .pAttachments = attachments,
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 1,
        .pDependencies = &dependency
    };

    VK_CHECK(vkCreateRenderPass(device->logical, &info, 0, &pass->pass))
}

void begin_renderpass(renderpass *pass, 
            VkCommandBuffer buff, 
            VkFramebuffer frameBuff, 
            VkClearValue clear, 
            VkExtent2D extent, 
            VkOffset2D offset) {
    VkRenderPassBeginInfo passBegin = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .clearValueCount = 1,
            .pClearValues = &clear,
            .framebuffer = frameBuff,
            .renderArea.offset = offset,
            .renderArea.extent = extent,
            .renderPass = pass->pass
        };
        vkCmdBeginRenderPass(buff, &passBegin, VK_SUBPASS_CONTENTS_INLINE);
}

void end_renderpass(VkCommandBuffer buff) {
    vkCmdEndRenderPass(buff);
}

void destroy_renderpass(renderpass* pass, device* device) {
    vkDestroyRenderPass(device->logical, pass->pass, 0);
}