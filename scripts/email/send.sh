#!/bin/bash
#
# curl-based email sender for admin@splitdo.app
# This replicates the functionality of ../test_admin_splitdo.sh using curl instead of sendmail
#
# SETUP:
# 1. Run generate_auth.py to create your auth credentials
# 2. Replace YOUR_USERNAME and YOUR_PASSWORD below with your actual credentials
# 3. Make this script executable: chmod +x send_test.sh
# 4. Run: ./send_test.sh

LOGO_PATH="/home/user/Projects/C/video_rendering/public/logo.webp"
RECIPIENT="ahmedmoti767@gmail.com"
FROM_EMAIL="admin@splitdo.app"
SMTP_SERVER="mail.gibgosolutions.com"
SMTP_PORT="587"

# ===== EDIT THESE VALUES =====
SMTP_USERNAME="admin@splitdo.app"  # e.g., admin@splitdo.app
SMTP_PASSWORD="Admin@Splitdo2025"  # Your actual password
# =============================

BOUNDARY="----=_Part_$(date +%s)_$(echo $RANDOM)"
BOUNDARY_ALT="----=_Alt_$(date +%s)_$(echo $RANDOM)"
BOUNDARY_REL="----=_Rel_$(date +%s)_$(echo $RANDOM)"

# Check if logo exists
if [ ! -f "$LOGO_PATH" ]; then
    echo "Error: Logo file not found at $LOGO_PATH"
    exit 1
fi

# Build the email content
EMAIL_CONTENT=$(cat <<EOF
From: ${FROM_EMAIL}
To: ${RECIPIENT}
Subject: 🎬 Video Rendering System Demo - Technical Achievement Showcase
MIME-Version: 1.0
Content-Type: multipart/mixed; boundary="${BOUNDARY}"

--${BOUNDARY}
Content-Type: multipart/related; boundary="${BOUNDARY_REL}"

--${BOUNDARY_REL}
Content-Type: multipart/alternative; boundary="${BOUNDARY_ALT}"

--${BOUNDARY_ALT}
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 7bit

Hello Ahmed Moti

I hope you are doing well.

I'm excited to share our latest technical achievement - a complete C-based video rendering system that demonstrates our engineering capabilities.

We've successfully developed a high-performance video generation pipeline using:
- FFmpeg libraries for professional video encoding
- Cairo graphics for smooth 2D rendering
- SVG support for scalable vector graphics
- Real-time frame-by-frame animation

The attached MP4 showcases a smooth 5-second animation rendered at 1920x1080 resolution, demonstrating our ability to create dynamic visual content programmatically.

This technology opens new possibilities for automated video generation, perfect for creating personalized content at scale.

📁 Source Code: https://github.com/Gibgoyt/video_rendering
🌐 Platform: https://e4fd89d1.cloudflare-frontend-9xj.pages.dev/

Kind regards,
SplitDo Team

--${BOUNDARY_ALT}
Content-Type: text/html; charset=UTF-8
Content-Transfer-Encoding: 7bit

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>SplitDo</title>
</head>
<body style="margin: 0; padding: 0; font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif; background-color: #5a4a42;">
    <table role="presentation" style="width: 100%; border-collapse: collapse; background-color: #5a4a42;">
        <tr>
            <td align="center" style="padding: 40px 20px;">
                <table role="presentation" style="width: 100%; max-width: 600px; border-collapse: collapse; background-color: #ffffff; box-shadow: 0 4px 12px rgba(0,0,0,0.2); border-radius: 8px; overflow: hidden;">

                    <!-- Header with gradient -->
                    <tr>
                        <td style="background: linear-gradient(135deg, #FF9B6B 0%, #6BBEBD 100%); padding: 40px 40px 30px 40px; text-align: center;">
                            <img src="cid:logo" alt="SplitDo" style="width: 120px; height: 120px; margin-bottom: 20px; border-radius: 16px; background-color: #5a4a42; padding: 10px;">
                            <h1 style="margin: 0; color: #ffffff; font-size: 28px; font-weight: 600; letter-spacing: -0.5px; text-shadow: 0 2px 4px rgba(0,0,0,0.1);">SplitDo</h1>
                            <div style="margin-top: 15px;">
                                <svg width="80" height="4" style="display: block; margin: 0 auto;">
                                    <rect width="80" height="4" rx="2" fill="#ffffff" opacity="0.5"/>
                                </svg>
                            </div>
                        </td>
                    </tr>

                    <!-- Decorative wave -->
                    <tr>
                        <td style="padding: 0; line-height: 0;">
                            <svg width="100%" height="30" viewBox="0 0 600 30" preserveAspectRatio="none" style="display: block;">
                                <path d="M0,15 Q150,0 300,15 T600,15 L600,30 L0,30 Z" fill="#6BBEBD"/>
                                <path d="M0,20 Q150,10 300,20 T600,20 L600,30 L0,30 Z" fill="#FF9B6B" opacity="0.3"/>
                            </svg>
                        </td>
                    </tr>

                    <!-- Main content -->
                    <tr>
                        <td style="padding: 50px 40px;">
                            <div style="margin-bottom: 10px;">
                                <svg width="40" height="40" viewBox="0 0 40 40" style="float: left; margin-right: 15px;">
                                    <circle cx="20" cy="20" r="18" fill="#fff4ed" stroke="#FF9B6B" stroke-width="2"/>
                                    <path d="M12 20 L18 26 L28 14" fill="none" stroke="#FF9B6B" stroke-width="3" stroke-linecap="round" stroke-linejoin="round"/>
                                </svg>
                            </div>
                            <h2 style="margin: 0 0 25px 0; color: #5a4a42; font-size: 24px; font-weight: 600;">Video Rendering System Demo</h2>

                            <div style="clear: both;"></div>

                            <p style="margin: 0 0 20px 0; color: #555555; font-size: 16px; line-height: 1.6;">
                                Hello Ahmed Moti, I hope you are doing well.
                            </p>

                            <p style="margin: 0 0 20px 0; color: #555555; font-size: 16px; line-height: 1.6;">
                                I'm excited to share our latest <strong>technical achievement</strong> - a complete C-based video rendering system that demonstrates our engineering capabilities.
                            </p>

                            <!-- Tech Stack Highlights -->
                            <div style="background-color: #f8f9fa; border-left: 4px solid #FF9B6B; padding: 20px; margin: 25px 0; border-radius: 4px;">
                                <h3 style="margin: 0 0 15px 0; color: #5a4a42; font-size: 18px; font-weight: 600;">🚀 Technology Stack</h3>
                                <ul style="margin: 0; padding-left: 20px; color: #555555; font-size: 15px; line-height: 1.6;">
                                    <li><strong>FFmpeg libraries</strong> for professional H.264 video encoding</li>
                                    <li><strong>Cairo graphics</strong> for smooth 2D rendering</li>
                                    <li><strong>SVG support</strong> for scalable vector graphics</li>
                                    <li><strong>Real-time</strong> frame-by-frame animation</li>
                                </ul>
                            </div>

                            <p style="margin: 0 0 20px 0; color: #555555; font-size: 16px; line-height: 1.6;">
                                The attached <strong>MP4 video</strong> showcases a smooth 5-second animation rendered at <strong>1920×1080 resolution</strong>, demonstrating our ability to create dynamic visual content programmatically.
                            </p>

                            <p style="margin: 0 0 25px 0; color: #555555; font-size: 16px; line-height: 1.6;">
                                This technology opens new possibilities for <strong>automated video generation</strong>, perfect for creating personalized content at scale.
                            </p>

                            <!-- GitHub Repository -->
                            <div style="background-color: #f8f9fa; border: 1px solid #e9ecef; border-radius: 8px; padding: 20px; margin: 25px 0;">
                                <div style="display: flex; align-items: center; margin-bottom: 10px;">
                                    <svg width="24" height="24" viewBox="0 0 24 24" fill="#333" style="margin-right: 10px;">
                                        <path d="M12 0C5.374 0 0 5.373 0 12 0 17.302 3.438 21.8 8.207 23.387c.599.111.793-.261.793-.577v-2.234c-3.338.726-4.033-1.416-4.033-1.416-.546-1.387-1.333-1.756-1.333-1.756-1.089-.745.083-.729.083-.729 1.205.084 1.839 1.237 1.839 1.237 1.07 1.834 2.807 1.304 3.492.997.107-.775.418-1.305.762-1.604-2.665-.305-5.467-1.334-5.467-5.931 0-1.311.469-2.381 1.236-3.221-.124-.303-.535-1.524.117-3.176 0 0 1.008-.322 3.301 1.23A11.509 11.509 0 0112 5.803c1.02.005 2.047.138 3.006.404 2.291-1.552 3.297-1.23 3.297-1.23.653 1.653.242 2.874.118 3.176.77.84 1.235 1.911 1.235 3.221 0 4.609-2.807 5.624-5.479 5.921.43.372.823 1.102.823 2.222v3.293c0 .319.192.694.801.576C20.566 21.797 24 17.3 24 12c0-6.627-5.373-12-12-12z"/>
                                    </svg>
                                    <h3 style="margin: 0; color: #333; font-size: 16px; font-weight: 600;">🔧 Open Source Code</h3>
                                </div>
                                <p style="margin: 0 0 12px 0; color: #666; font-size: 14px; line-height: 1.5;">
                                    Explore the complete source code, build instructions, and technical documentation on GitHub:
                                </p>
                                <a href="https://github.com/Gibgoyt/video_rendering" style="display: inline-block; color: #0366d6; text-decoration: none; font-size: 14px; font-weight: 500; font-family: 'SFMono-Regular', Consolas, 'Liberation Mono', Menlo, monospace; background-color: #f6f8fa; padding: 6px 12px; border-radius: 6px; border: 1px solid #d0d7de;">
                                    github.com/Gibgoyt/video_rendering
                                </a>
                            </div>

                            <!-- CTA Buttons -->
                            <table role="presentation" style="margin: 0 0 30px 0; border-collapse: collapse;">
                                <tr>
                                    <td style="padding-right: 10px;">
                                        <table role="presentation" style="border-collapse: collapse;">
                                            <tr>
                                                <td style="border-radius: 8px; background: linear-gradient(135deg, #FF9B6B 0%, #6BBEBD 100%); text-align: center;">
                                                    <a href="https://e4fd89d1.cloudflare-frontend-9xj.pages.dev/" style="display: inline-block; padding: 14px 28px; color: #ffffff; text-decoration: none; font-size: 15px; font-weight: 600; border-radius: 8px;">
                                                        View Our Platform
                                                    </a>
                                                </td>
                                            </tr>
                                        </table>
                                    </td>
                                    <td style="padding-left: 10px;">
                                        <table role="presentation" style="border-collapse: collapse;">
                                            <tr>
                                                <td style="border-radius: 8px; background-color: #333; text-align: center;">
                                                    <a href="https://github.com/Gibgoyt/video_rendering" style="display: inline-block; padding: 14px 28px; color: #ffffff; text-decoration: none; font-size: 15px; font-weight: 600; border-radius: 8px;">
                                                        View Source Code
                                                    </a>
                                                </td>
                                            </tr>
                                        </table>
                                    </td>
                                </tr>
                            </table>

                            <!-- Decorative divider -->
                            <div style="margin: 30px 0;">
                                <svg width="100%" height="2" style="display: block;">
                                    <defs>
                                        <linearGradient id="grad1" x1="0%" y1="0%" x2="100%" y2="0%">
                                            <stop offset="0%" style="stop-color:#ffffff;stop-opacity:1" />
                                            <stop offset="50%" style="stop-color:#FF9B6B;stop-opacity:1" />
                                            <stop offset="100%" style="stop-color:#ffffff;stop-opacity:1" />
                                        </linearGradient>
                                    </defs>
                                    <rect width="100%" height="2" fill="url(#grad1)" />
                                </svg>
                            </div>

                            <!-- Signature -->
                            <table role="presentation" style="margin-top: 30px; border-collapse: collapse;">
                                <tr>
                                    <td style="padding: 0;">
                                        <p style="margin: 0 0 5px 0; color: #5a4a42; font-size: 15px; font-weight: 600;">Kind regards,</p>
                                        <p style="margin: 0; color: #FF9B6B; font-size: 18px; font-weight: 700;">SplitDo Team</p>
                                    </td>
                                </tr>
                            </table>
                        </td>
                    </tr>

                    <!-- Feature highlights -->
                    <tr>
                        <td style="padding: 0 40px 40px 40px;">
                            <table role="presentation" style="width: 100%; border-collapse: collapse;">
                                <tr>
                                    <td style="width: 33.33%; text-align: center; padding: 20px 10px; background-color: #fff4ed; border-radius: 8px;">
                                        <svg width="48" height="48" viewBox="0 0 48 48" style="margin-bottom: 10px;">
                                            <circle cx="24" cy="24" r="22" fill="#ffe8d6"/>
                                            <path d="M24 14 L24 34 M14 24 L34 24" stroke="#FF9B6B" stroke-width="3" stroke-linecap="round"/>
                                            <circle cx="24" cy="24" r="3" fill="#FF9B6B"/>
                                        </svg>
                                        <p style="margin: 0; color: #5a4a42; font-size: 12px; font-weight: 600;">Split Bills</p>
                                    </td>
                                    <td style="width: 33.33%; text-align: center; padding: 20px 10px; background-color: #e8f7f6; border-radius: 8px;">
                                        <svg width="48" height="48" viewBox="0 0 48 48" style="margin-bottom: 10px;">
                                            <circle cx="24" cy="24" r="22" fill="#d4efee"/>
                                            <path d="M18 24 L22 28 L30 18" fill="none" stroke="#6BBEBD" stroke-width="3" stroke-linecap="round" stroke-linejoin="round"/>
                                        </svg>
                                        <p style="margin: 0; color: #5a4a42; font-size: 12px; font-weight: 600;">Track Expenses</p>
                                    </td>
                                    <td style="width: 33.33%; text-align: center; padding: 20px 10px; background-color: #f5f0ec; border-radius: 8px;">
                                        <svg width="48" height="48" viewBox="0 0 48 48" style="margin-bottom: 10px;">
                                            <circle cx="24" cy="24" r="22" fill="#ebe2d9"/>
                                            <path d="M24 16 L28 22 L24 28 L20 22 Z" fill="none" stroke="#5a4a42" stroke-width="2.5"/>
                                            <circle cx="24" cy="22" r="2" fill="#5a4a42"/>
                                        </svg>
                                        <p style="margin: 0; color: #5a4a42; font-size: 12px; font-weight: 600;">Settle Up</p>
                                    </td>
                                </tr>
                            </table>
                        </td>
                    </tr>

                    <!-- Footer -->
                    <tr>
                        <td style="background: linear-gradient(135deg, #5a4a42 0%, #6BBEBD 100%); padding: 30px 40px; text-align: center;">
                            <div style="margin-bottom: 20px;">
                                <svg width="200" height="2" style="display: block; margin: 0 auto 20px auto;">
                                    <rect width="200" height="2" fill="#ffffff" opacity="0.3"/>
                                </svg>
                            </div>

                            <p style="margin: 0 0 10px 0; color: #ffffff; font-size: 14px; opacity: 0.9;">
                                <strong>SplitDo</strong>
                            </p>
                            <p style="margin: 0 0 5px 0; color: #ffffff; font-size: 13px; opacity: 0.7;">
                                admin@splitdo.app
                            </p>
                            <p style="margin: 0; color: #ffffff; font-size: 12px; opacity: 0.6;">
                                © 2025 SplitDo. All rights reserved.
                            </p>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
    </table>
</body>
</html>

--${BOUNDARY_ALT}--

--${BOUNDARY_REL}
Content-Type: image/webp
Content-Transfer-Encoding: base64
Content-ID: <logo>
Content-Disposition: inline; filename="logo.webp"

$(base64 "$LOGO_PATH")

--${BOUNDARY_REL}--

--${BOUNDARY}
Content-Type: video/mp4
Content-Transfer-Encoding: base64
Content-Disposition: attachment; filename="video_demo.mp4"

$(base64 "output.mp4")

--${BOUNDARY}--
EOF
)

# Send the email using curl
echo "Sending email from ${FROM_EMAIL} to ${RECIPIENT}..."
echo "$EMAIL_CONTENT" | curl --url "smtp://${SMTP_SERVER}:${SMTP_PORT}" \
  --mail-from "${FROM_EMAIL}" \
  --mail-rcpt "${RECIPIENT}" \
  --user "${SMTP_USERNAME}:${SMTP_PASSWORD}" \
  --ssl-reqd \
  --insecure \
  --upload-file - \
  -v

if [ $? -eq 0 ]; then
    echo "Email sent successfully!"
else
    echo "Failed to send email. Check your credentials and SMTP configuration."
    exit 1
fi
