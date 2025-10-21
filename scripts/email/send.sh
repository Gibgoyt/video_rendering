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
Subject: Greetings from SplitDo
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

This is a test message from SplitDo to verify our email delivery system is working correctly.

Please look at the site updates so far: https://e4fd89d1.cloudflare-frontend-9xj.pages.dev/

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
                            <h2 style="margin: 0 0 25px 0; color: #5a4a42; font-size: 24px; font-weight: 600;">Hello Ahmed Moti</h2>

                            <div style="clear: both;"></div>

                            <p style="margin: 0 0 20px 0; color: #555555; font-size: 16px; line-height: 1.6;">
                                I hope you are doing well.
                            </p>

                            <p style="margin: 0 0 20px 0; color: #555555; font-size: 16px; line-height: 1.6;">
                                This is a test message from <strong>SplitDo</strong> to verify our email delivery system is working correctly.
                            </p>

                            <p style="margin: 0 0 25px 0; color: #555555; font-size: 16px; line-height: 1.6;">
                                Please look at the site updates so far:
                            </p>

                            <!-- CTA Button -->
                            <table role="presentation" style="margin: 0 0 30px 0; border-collapse: collapse;">
                                <tr>
                                    <td style="border-radius: 8px; background: linear-gradient(135deg, #FF9B6B 0%, #6BBEBD 100%); text-align: center;">
                                        <a href="https://e4fd89d1.cloudflare-frontend-9xj.pages.dev/" style="display: inline-block; padding: 14px 32px; color: #ffffff; text-decoration: none; font-size: 16px; font-weight: 600; border-radius: 8px;">
                                            View Site Updates
                                        </a>
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
