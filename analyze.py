import arviz as az
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy.stats import gaussian_kde


def analyze_mcmc(csv_path: str):
    df = pd.read_csv(csv_path)

    num_chains: int = len(df["chain"].unique())
    samples_per_chain: int = len(df) // num_chains
    param_cols = [c for c in df.columns if c.startswith("p")]
    num_params = len(param_cols)

    print(f"Detected {num_chains} chains with {samples_per_chain} samples each.")

    df_sorted = df.sort_values(["chain", "sample"])
    posterior_dict = {
        p: df_sorted[p].to_numpy().reshape((num_chains, samples_per_chain))
        for p in param_cols
    }

    posteriors = {"posterior": posterior_dict}
    dataset = az.from_dict(posteriors)

    # for high-dim, we limit how many parameters we show per page or use subplots
    az.plot_trace(dataset, var_names=param_cols)  # Show first 6 to keep it clean
    plt.suptitle("Trace Plots")
    plt.tight_layout()
    plt.savefig("./output/traces.png", dpi=200)
    print("Traces saved to './output/traces.png'")

    # summary statistics (the most important part for high-dim)
    summary = az.summary(dataset)
    with open("./output/diagnostics.txt", "w") as f:
        f.write("\n--- Diagnostic Summary ---\n")
        f.write(summary.to_string())
    print("Diagnostic results written to './output/diagnostics.txt'")

    fig, axes = plt.subplots(
        num_params, num_params, figsize=(num_params * 1.8, num_params * 1.8)
    )

    for i in range(num_params):
        for j in range(num_params):
            ax = axes[i, j]
            p_row, p_col = param_cols[i], param_cols[j]

            if i == j:
                # Diagonal: Histogram with chain colors if you want
                ax.hist(df[p_row], bins=40, color="#440154", alpha=0.7, density=True)
                ax.set_title(p_row, fontsize=12, fontweight="bold")
            elif i > j:
                # Lower Triangle: Density-Colored Scatter
                x = df[p_col].to_numpy()
                y = df[p_row].to_numpy()

                # Performance trick: only calculate density for a subset if data is huge
                # but with your ESS, let's try 2000 points for the visual
                idx = np.random.choice(
                    np.arange(len(x)), size=min(2000, len(x)), replace=False
                )
                x_sub, y_sub = x[idx], y[idx]

                try:
                    xy = np.vstack([x_sub, y_sub])
                    z = gaussian_kde(xy)(xy)  # Calculate density for coloring
                    # Sort points so highest density is on top
                    sort_idx = z.argsort()
                    x_sub, y_sub, z = x_sub[sort_idx], y_sub[sort_idx], z[sort_idx]

                    ax.scatter(x_sub, y_sub, c=z, s=3, cmap="viridis", alpha=0.6)
                except Exception as ex:
                    err = "Matrix found to be singular!"
                    message = err.format(type(ex).__name__)
                    print(message)
            else:
                ax.axis("off")

            # Polish the axes
            if i < num_params - 1:
                ax.set_xticks([])
            if j > 0:
                ax.set_yticks([])
            ax.tick_params(labelsize=8)

    plt.tight_layout()
    plt.savefig("./output/corr_results.png", dpi=200)
    print("Pairwise correlations saved to './output/corr_results.png'")


if __name__ == "__main__":
    analyze_mcmc("./output/output.csv")
